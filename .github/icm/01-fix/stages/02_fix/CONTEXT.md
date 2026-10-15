# 02_fix — change the cause, not the symptom

One job: the smallest change at the defect's cause that turns the reproduction green and keeps every existing sketch compiling.

## Inputs
- Working (this run): `.github/icm/01-fix/stages/01_reproduce/output/reproduction.md`
- Reference (every run): `.github/icm/_shared/architecture.md`
- Reference (only when the fix is in a workflow or a manifest): `.github/icm/_shared/build-and-test.md`

Do NOT load: the other pipelines, other runs' output, `.github/icm/_shared/documents.md`, `resources/`, `bin/`.

## Process
1. Confirm the cause in the code before changing anything; if it moved, record where it really is.
2. Change the cause, in the style of the code around it (descriptive names, readable descriptions for register values, `DEBUG_PRINT` for diagnostics). Keep the public API, the constructor overloads and the three include names working ([architecture.md](../../../_shared/architecture.md) "Things that are easy to break").
3. If the change is inside a platform branch (`ACTIVATE_SOFTWARE_SERIAL`, `HARDWARE_SERIAL_SELECTABLE_PIN`, a `FREQUENCY_*` band), compile the reproduction for a board on each side of it.
4. Run the reproduction (green now) and compile every example the change touches for AVR, ESP32 and ESP8266.

## Outputs
- change.md → output/ — the cause in one paragraph; each file changed and why; the reproduction's passing output; the boards compiled; anything the fix deliberately leaves alone.

## Human check
A person reads the diff beside the reproduction: it removes the cause, keeps existing sketches compiling, and changes no public signature. Edit `change.md` if the explanation is wrong; stage 03 reads it.
