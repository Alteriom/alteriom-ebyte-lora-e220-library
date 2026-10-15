/*
 * Native unit tests for the library's own code.
 *
 * LoRa_E220.cpp is compiled into the test binary by library_source.cpp, on top
 * of the minimal Arduino core in test/native_shim/. A FakeE220 stands in for
 * the module on the UART: it records every byte the library writes and answers
 * the register-read commands the way the E220 datasheet describes
 * (C1 <address> <length> -> C1 <address> <length> <data...>).
 *
 * Run with: pio test -e native
 */
#include <unity.h>

#include "LoRa_E220.h"

namespace {

const byte AUX_PIN = 4;
const byte M0_PIN = 5;
const byte M1_PIN = 6;

// Register file the fake module answers from: ADDH ADDL SPED OPTION CHAN
// TRANSMISSION_MODE CRYPT_H CRYPT_L (register 0x00..0x07) and the product
// information at 0x08..0x0A.
class FakeE220 : public HardwareSerial {
public:
	uint8_t registers[11] = {
		0x12, 0x34,  // ADDH, ADDL
		0x62,        // SPED: UART 9600 (011), parity 8N1 (00), air 2.4k (010)
		0x00,        // OPTION: 200 bytes, noise RSSI off, 22 dBm
		0x17,        // CHAN 23 -> 410 + 23 = 433 MHz
		0x40,        // TRANSMISSION_MODE: fixed transmission
		0x00, 0x00,  // CRYPT (write-only on the module, read back as 0)
		0x20, 0x0B, 0x0E  // model, version, features
	};
	bool answer = true;            // false: the module stays silent
	bool truncatedAnswer = false;  // true: only the 3-byte header comes back

	void resetCapture() { written.clear(); rx.clear(); }

protected:
	void onWrite(const uint8_t *data, size_t size) override {
		Stream::onWrite(data, size);
		if (!answer || size != 3 || data[0] != READ_CONFIGURATION) return;
		uint8_t address = data[1];
		uint8_t length = data[2];
		if (address + length > sizeof(registers)) return;
		uint8_t header[3] = {RETURNED_COMMAND, address, length};
		feed(header, 3);
		if (!truncatedAnswer) feed(&registers[address], length);
	}
};

FakeE220 *uart;

void assert_mode_pins(int m0, int m1) {
	TEST_ASSERT_EQUAL_INT(m0, digitalRead(M0_PIN));
	TEST_ASSERT_EQUAL_INT(m1, digitalRead(M1_PIN));
}

}  // namespace

void setUp(void) {
	native_shim::reset();
	uart = new FakeE220();
}

void tearDown(void) {
	delete uart;
	uart = nullptr;
}

//-----------------------------------------------------------------------------
// Descriptions (includes/statesNaming.h and the configuration structures)
//-----------------------------------------------------------------------------

void test_response_status_descriptions() {
	ResponseStatus status;
	status.code = E220_SUCCESS;
	TEST_ASSERT_EQUAL_STRING("Success", status.getResponseDescription().c_str());
	status.code = ERR_E220_PACKET_TOO_BIG;
	TEST_ASSERT_EQUAL_STRING("The device support only 200byte of data transmission!",
		status.getResponseDescription().c_str());
	TEST_ASSERT_EQUAL_STRING("Invalid status!", getResponseDescriptionByParams(0).c_str());
}

void test_configuration_channel_description() {
	Configuration configuration;
	configuration.CHAN = 23;
	// OPERATING_FREQUENCY defaults to 410 MHz (the 400/433 MHz band).
	TEST_ASSERT_EQUAL_STRING("433MHz", configuration.getChannelDescription().c_str());
}

void test_speed_descriptions() {
	Speed speed;
	speed.uartBaudRate = UART_BPS_9600;
	speed.airDataRate = AIR_DATA_RATE_010_24;
	TEST_ASSERT_EQUAL_STRING("9600bps (default)", speed.getUARTBaudRateDescription().c_str());
	TEST_ASSERT_EQUAL_STRING("2.4kbps (default)", speed.getAirDataRateDescription().c_str());
}

//-----------------------------------------------------------------------------
// Start-up and operating modes
//-----------------------------------------------------------------------------

void test_begin_opens_the_uart_and_enters_normal_mode() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);  // module idle

	TEST_ASSERT_TRUE(e220.begin());
	TEST_ASSERT_TRUE(uart->started);
	TEST_ASSERT_EQUAL_UINT32(9600, uart->baud);
	TEST_ASSERT_EQUAL_INT(OUTPUT, native_shim::pin_modes()[M0_PIN]);
	TEST_ASSERT_EQUAL_INT(OUTPUT, native_shim::pin_modes()[M1_PIN]);
	TEST_ASSERT_EQUAL_INT(INPUT, native_shim::pin_modes()[AUX_PIN]);
	TEST_ASSERT_EQUAL_INT(MODE_0_NORMAL, e220.getMode());
	assert_mode_pins(LOW, LOW);
}

void test_set_mode_drives_m0_and_m1() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();

	TEST_ASSERT_EQUAL_INT(E220_SUCCESS, e220.setMode(MODE_1_WOR_TRANSMITTER));
	assert_mode_pins(HIGH, LOW);
	TEST_ASSERT_EQUAL_INT(E220_SUCCESS, e220.setMode(MODE_2_WOR_RECEIVER));
	assert_mode_pins(LOW, HIGH);
	TEST_ASSERT_EQUAL_INT(E220_SUCCESS, e220.setMode(MODE_3_CONFIGURATION));
	assert_mode_pins(HIGH, HIGH);
	TEST_ASSERT_EQUAL_INT(MODE_3_CONFIGURATION, e220.getMode());
}

void test_set_mode_times_out_while_aux_stays_low() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, LOW);  // module busy, never finishes

	TEST_ASSERT_EQUAL_INT(ERR_E220_TIMEOUT, e220.setMode(MODE_3_CONFIGURATION));
	TEST_ASSERT_NOT_EQUAL(MODE_3_CONFIGURATION, e220.getMode());
}

//-----------------------------------------------------------------------------
// Reading the module's registers
//-----------------------------------------------------------------------------

void test_get_configuration_reads_and_parses_the_registers() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();
	uart->resetCapture();

	ResponseStructContainer rc = e220.getConfiguration();
	TEST_ASSERT_EQUAL_INT(E220_SUCCESS, rc.status.code);

	const uint8_t expectedCommand[3] = {READ_CONFIGURATION, REG_ADDRESS_CFG, PL_CONFIGURATION};
	TEST_ASSERT_EQUAL_size_t(3, uart->written.size());
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedCommand, uart->written.data(), 3);

	Configuration *configuration = (Configuration *)rc.data;
	TEST_ASSERT_EQUAL_HEX8(0x12, configuration->ADDH);
	TEST_ASSERT_EQUAL_HEX8(0x34, configuration->ADDL);
	TEST_ASSERT_EQUAL_UINT8(23, configuration->CHAN);
	TEST_ASSERT_EQUAL_UINT8(UART_BPS_9600, configuration->SPED.uartBaudRate);
	TEST_ASSERT_EQUAL_UINT8(MODE_00_8N1, configuration->SPED.uartParity);
	TEST_ASSERT_EQUAL_UINT8(AIR_DATA_RATE_010_24, configuration->SPED.airDataRate);
	TEST_ASSERT_EQUAL_UINT8(FT_FIXED_TRANSMISSION, configuration->TRANSMISSION_MODE.fixedTransmission);
	TEST_ASSERT_EQUAL_STRING("433MHz", configuration->getChannelDescription().c_str());

	// Back in the mode it was in before the read.
	TEST_ASSERT_EQUAL_INT(MODE_0_NORMAL, e220.getMode());
	assert_mode_pins(LOW, LOW);
	rc.close();
}

void test_get_configuration_refuses_a_uart_other_than_9600() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_19200);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();
	uart->resetCapture();

	ResponseStructContainer rc = e220.getConfiguration();
	TEST_ASSERT_EQUAL_INT(ERR_E220_WRONG_UART_CONFIG, rc.status.code);
	TEST_ASSERT_EQUAL_size_t(0, uart->written.size());
}

void test_get_configuration_reports_a_silent_module() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();
	uart->answer = false;

	ResponseStructContainer rc = e220.getConfiguration();
	TEST_ASSERT_EQUAL_INT(ERR_E220_NO_RESPONSE_FROM_DEVICE, rc.status.code);
	TEST_ASSERT_EQUAL_INT(MODE_0_NORMAL, e220.getMode());
	rc.close();
}

void test_get_configuration_reports_a_short_answer() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();
	uart->truncatedAnswer = true;

	ResponseStructContainer rc = e220.getConfiguration();
	TEST_ASSERT_EQUAL_INT(ERR_E220_DATA_SIZE_NOT_MATCH, rc.status.code);
	rc.close();
}

void test_get_module_information_reads_the_product_registers() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();
	uart->resetCapture();

	ResponseStructContainer rc = e220.getModuleInformation();
	TEST_ASSERT_EQUAL_INT(E220_SUCCESS, rc.status.code);

	const uint8_t expectedCommand[3] = {READ_CONFIGURATION, REG_ADDRESS_PID, PL_PID};
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedCommand, uart->written.data(), 3);

	ModuleInformation *information = (ModuleInformation *)rc.data;
	TEST_ASSERT_EQUAL_HEX8(0x20, information->model);
	TEST_ASSERT_EQUAL_HEX8(0x0B, information->version);
	TEST_ASSERT_EQUAL_HEX8(0x0E, information->features);
	rc.close();
}

void test_set_configuration_writes_the_command_and_the_registers() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();
	uart->resetCapture();

	Configuration configuration;
	configuration.ADDH = 0x00;
	configuration.ADDL = 0x03;
	configuration.CHAN = 23;
	e220.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);

	// Only the frame the library sends is asserted: C0 00 08 then the eight
	// register bytes, in the order the module expects them.
	TEST_ASSERT_EQUAL_size_t(sizeof(Configuration), uart->written.size());
	TEST_ASSERT_EQUAL_HEX8(WRITE_CFG_PWR_DWN_SAVE, uart->written[0]);
	TEST_ASSERT_EQUAL_HEX8(REG_ADDRESS_CFG, uart->written[1]);
	TEST_ASSERT_EQUAL_HEX8(PL_CONFIGURATION, uart->written[2]);
	TEST_ASSERT_EQUAL_HEX8(0x00, uart->written[3]);
	TEST_ASSERT_EQUAL_HEX8(0x03, uart->written[4]);
	TEST_ASSERT_EQUAL_UINT8(23, uart->written[7]);
	TEST_ASSERT_EQUAL_INT(MODE_0_NORMAL, e220.getMode());
}

//-----------------------------------------------------------------------------
// Sending and receiving
//-----------------------------------------------------------------------------

void test_send_message_writes_the_text() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();
	uart->resetCapture();

	ResponseStatus status = e220.sendMessage(String("hello"));
	TEST_ASSERT_EQUAL_INT(E220_SUCCESS, status.code);
	TEST_ASSERT_EQUAL_size_t(5, uart->written.size());
	TEST_ASSERT_EQUAL_MEMORY("hello", uart->written.data(), 5);
}

void test_send_fixed_message_prefixes_address_and_channel() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();
	uart->resetCapture();

	ResponseStatus status = e220.sendFixedMessage(0x00, 0x03, 23, String("hi"));
	TEST_ASSERT_EQUAL_INT(E220_SUCCESS, status.code);
	const uint8_t expected[5] = {0x00, 0x03, 23, 'h', 'i'};
	TEST_ASSERT_EQUAL_size_t(5, uart->written.size());
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, uart->written.data(), 5);

	uart->resetCapture();
	e220.sendBroadcastFixedMessage(23, String("hi"));
	TEST_ASSERT_EQUAL_HEX8(BROADCAST_ADDRESS, uart->written[0]);
	TEST_ASSERT_EQUAL_HEX8(BROADCAST_ADDRESS, uart->written[1]);
}

void test_send_message_refuses_a_packet_over_the_limit() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();
	uart->resetCapture();

	uint8_t payload[MAX_SIZE_TX_PACKET + 3] = {0};
	ResponseStatus status = e220.sendMessage(payload, sizeof(payload));
	TEST_ASSERT_EQUAL_INT(ERR_E220_PACKET_TOO_BIG, status.code);
	TEST_ASSERT_EQUAL_size_t(0, uart->written.size());
}

void test_receive_message_returns_the_text_and_the_rssi() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();

	const uint8_t text[] = {'p', 'i', 'n', 'g'};
	uart->feed(text, sizeof(text));
	ResponseContainer plain = e220.receiveMessage();
	TEST_ASSERT_EQUAL_INT(E220_SUCCESS, plain.status.code);
	TEST_ASSERT_EQUAL_STRING("ping", plain.data.c_str());

	const uint8_t textWithRssi[] = {'p', 'o', 'n', 'g', 0x5A};
	uart->feed(textWithRssi, sizeof(textWithRssi));
	ResponseContainer withRssi = e220.receiveMessageRSSI();
	TEST_ASSERT_EQUAL_STRING("pong", withRssi.data.c_str());
	TEST_ASSERT_EQUAL_HEX8(0x5A, withRssi.rssi);
	TEST_ASSERT_EQUAL_INT(0, e220.available());
}

void test_receive_message_of_a_fixed_size() {
	LoRa_E220 e220(uart, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);
	digitalWrite(AUX_PIN, HIGH);
	e220.begin();

	const uint8_t frame[] = {1, 2, 3, 4};
	uart->feed(frame, sizeof(frame));
	ResponseStructContainer rc = e220.receiveMessage(4);
	TEST_ASSERT_EQUAL_INT(E220_SUCCESS, rc.status.code);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(frame, (uint8_t *)rc.data, 4);
	rc.close();

	uart->feed(frame, 2);
	rc = e220.receiveMessage(4);
	TEST_ASSERT_EQUAL_INT(ERR_E220_DATA_SIZE_NOT_MATCH, rc.status.code);
	rc.close();
}

void test_reset_module_is_not_implemented() {
	LoRa_E220 e220(uart, UART_BPS_RATE_9600);
	TEST_ASSERT_EQUAL_INT(ERR_E220_NOT_IMPLEMENT, e220.resetModule().code);
}

int main(int argc, char **argv) {
	UNITY_BEGIN();

	RUN_TEST(test_response_status_descriptions);
	RUN_TEST(test_configuration_channel_description);
	RUN_TEST(test_speed_descriptions);

	RUN_TEST(test_begin_opens_the_uart_and_enters_normal_mode);
	RUN_TEST(test_set_mode_drives_m0_and_m1);
	RUN_TEST(test_set_mode_times_out_while_aux_stays_low);

	RUN_TEST(test_get_configuration_reads_and_parses_the_registers);
	RUN_TEST(test_get_configuration_refuses_a_uart_other_than_9600);
	RUN_TEST(test_get_configuration_reports_a_silent_module);
	RUN_TEST(test_get_configuration_reports_a_short_answer);
	RUN_TEST(test_get_module_information_reads_the_product_registers);
	RUN_TEST(test_set_configuration_writes_the_command_and_the_registers);

	RUN_TEST(test_send_message_writes_the_text);
	RUN_TEST(test_send_fixed_message_prefixes_address_and_channel);
	RUN_TEST(test_send_message_refuses_a_packet_over_the_limit);
	RUN_TEST(test_receive_message_returns_the_text_and_the_rssi);
	RUN_TEST(test_receive_message_of_a_fixed_size);
	RUN_TEST(test_reset_module_is_not_implemented);

	return UNITY_END();
}
