# AGENTS.md — alteriom-ebyte-lora-e220-library

Alteriom's fork of Renzo Mischianti's Arduino library for EBYTE E220 LoRa modules (LLCC68 chip, UART, M0/M1/AUX pins): configuration, transparent, fixed and broadcast transmission, RSSI and Wake-on-Radio, on AVR, ESP32, ESP8266, STM32, SAMD and RP2040. Public, MIT. It ships as `Alteriom_EByte_LoRa_E220` to the PlatformIO registry and npm from a `v*` tag, with a GitHub release and a Docsify site on GitHub Pages; Arduino Library Manager is not listed yet. Humans start at `README.md`. This file is the entry for every agent: Claude Code reads it through `CLAUDE.md`, and Copilot is pointed here by `.github/copilot-instructions.md`.

## Where things live

| Path | What it holds |
|---|---|
| `LoRa_E220.h`, `LoRa_E220.cpp`, `includes/statesNaming.h` | the library: the `LoRa_E220` class, its structs and the register enums — mapped in [architecture.md](.github/icm/_shared/architecture.md) |
| `Alteriom_EByte_LoRa_E220.h`, `EByte_LoRa_E220_library.h` | include wrappers; the first is what `library.properties` advertises |
| `library.properties`, `library.json`, `package.json` | the Arduino, PlatformIO and npm manifests; the version is in all three and in `LoRa_E220.h` |
| `examples/`, `test/` | one sketch per feature (`NN_name/NN_name.ino`); the native Unity tests (`pio test -e native`) |
| `docs/` | the Docsify site published to GitHub Pages — indexed in [documents.md](.github/icm/_shared/documents.md) |
| `scripts/`, `.github/workflows/` | version and validation scripts; CI, release, registry publishing, docs and wiki workflows, `icm-check.yml` |
| `resources/` | the EBYTE manuals and pictures. Never read it |
| [`.github/icm/_shared/`](.github/icm/_shared/CONTEXT.md) | what every job below draws on, one file per question |
| [`.github/icm/01-fix/`](.github/icm/01-fix/CLAUDE.md) | fix a defect in the library, an example or a workflow |
| [`.github/icm/02-release/`](.github/icm/02-release/CLAUDE.md) | cut a version and see it into every registry (a person tags it) |
| [`.github/icm/03-deps/`](.github/icm/03-deps/CLAUDE.md) | review a Dependabot bump |

## Route by what just happened

| Route | If | Go to | Then stop at |
|---|---|---|---|
| triage | an issue or report arrives and its kind is unclear | read it whole (`gh issue view <n> --comments`), then take the row below that fits | the row chosen, said to the person |
| fix | a defect is reported (a module answering wrongly, a sketch that does not compile, a failing test) | [01_reproduce](.github/icm/01-fix/stages/01_reproduce/CONTEXT.md) | a person has seen the reproduction fail |
| ci-red | `Build and Test Library` is red on `main`, or a release or publishing run failed | [01_reproduce](.github/icm/01-fix/stages/01_reproduce/CONTEXT.md), with the run log as the report | a person has seen the reproduction fail |
| release | a version is to be released | [01_plan](.github/icm/02-release/stages/01_plan/CONTEXT.md) | a person approves the version number |
| registry-check | a released version is missing from a registry, or the registries disagree | [02_publish](.github/icm/02-release/stages/02_publish/CONTEXT.md), from step 3, for that version | a person has seen the version in each registry |
| deps | Dependabot opened a pull request | [01_review](.github/icm/03-deps/stages/01_review/CONTEXT.md) | a person decides to merge it or not |
| docs | a page of the site or `README.md` is wrong and no code has to change | [documents.md](.github/icm/_shared/documents.md) | a person reads the edit beside the owning file |
| next | a stage's output was approved | the next numbered stage of the same job | a person reads that stage's output |
| status | asked for status | scan `.github/icm/*/stages/*/output/` | report what exists |

## Agents connecting remotely

This repository becomes the ICM workspace `alteriom-ebyte-lora-e220-library` on MemoryRelay, created by a person from this repository's `main`; the Alteriom hub `portfolio` then delegates to it. Until the workspace exists, an agent on a clone reads these files directly: this file is the entry and the table above is the route. Once it exists, an agent that is not on a clone reaches the same files over MCP at `https://api.memoryrelay.net/mcp` with a key in `MEMORYRELAY_API_KEY`; none of it holds a secret.

- Start with `icm_context_build` for the route that fits (`{kind: "route", id: "fix"}`), not with the whole repository.
- Report what you read with `icm_report_reads` when a stage ends, and what the task came to with `icm_report_outcome`.
- Never write through MCP: a change is a branch and a pull request here, read and merged by a person.

## Rules

- Nothing moves to the next stage until a person has read the output of the last one.
- A stage writes to its own `output/`, which is not committed: one run per worktree or branch; clear that job's `stages/*/output/` when its pull request merges or is abandoned.
- `.github/icm/` points at the files that own each fact. When they disagree, the owning file is right: correct `.github/icm/` in the same pull request and bump its `verified_commit`.
- No stage pushes a tag, publishes a package or submits to a registry: a person does ([release.md](.github/icm/_shared/release.md)).
- Keep the public API backward compatible: existing sketches must still compile.
- Public repository: no host name, address, secret or private repository path of ours in code or in these files.
