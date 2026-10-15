# 01_reproduce — reproduce the reported defect

One job: turn the report into something that fails on demand, for the reason reported: a compile error on a named board, a wrong value from a method, a failed workflow run.

## Inputs
- Working (this run): the report — read whole with what it links: an issue (`gh issue view <n> --comments`), a red run (`gh run view <run-id> --log-failed`), a user's sketch, board and module variant.
- Reference (every run): `.github/icm/_shared/architecture.md`
- Reference (every run): `.github/icm/_shared/build-and-test.md`
- Reference (only when the red run is a release or publishing workflow): `.github/icm/_shared/release.md`

Do NOT load: the other pipelines, other runs' output, `resources/`, `bin/`, the September 2025 status pages.

## Process
1. Restate the report as observed versus expected, with the board (FQBN), the core version, the module variant and the constructor the sketch uses.
2. Find the code path with [architecture.md](../../../_shared/architecture.md) and name the suspected cause as `path:line`. A constructor or serial problem is usually a platform branch of `LoRa_E220.h`.
3. Reproduce it the way that proves it: `arduino-cli compile --fqbn <board> --library . <sketch>` for a compile error; for a wrong value, the smallest sketch that prints it, compiled for the reported board, and the expected value from `includes/statesNaming.h` or the EBYTE manual's register table. `pio test -e native` cannot reproduce a library defect: it tests a mock ([build-and-test.md](../../../_shared/build-and-test.md) "Evidence").
4. Keep the failing output. If the defect needs a module on the air and none is available, say so: the reproduction is the user's serial log plus the code path, and a person decides whether that is enough.

## Outputs
- reproduction.md → output/ — the restated report; board, core, module and constructor; suspected cause (`path:line`); the command or sketch; the failing output; what it does not cover (no module run).

## Human check
A person runs the command, or reads the output and the code path, and confirms it fails for the reported reason and not a neighbouring one.
