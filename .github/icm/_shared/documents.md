---
verified_at: 2026-10-15
verified_commit: 6d202404f514ab13e80a302ead766aca96e29afd
---

# Documents — what a person reads here, and who owns its facts

The agent-facing files are `AGENTS.md` (the entry), this folder and `.github/copilot-instructions.md`. Everything below is a document a person reads: each restates facts that the library's headers, an example or a workflow owns, and the owner is right when they disagree. Each linked document is pinned in the workspace, so an agent can be given one by path.

## The site

`docs/` is a Docsify site (`docs/index.html`, navigation in `docs/_sidebar.md` and `docs/_navbar.md`), deployed to GitHub Pages by `.github/workflows/documentation.yml` on a push to `main` that touches `docs/**` or any Markdown file; a pull request only checks it. `update-wiki.yml` also copies pages into the repository wiki on the same pushes. To correct a page: edit it under `docs/`, check every changed claim against its owner (compile the snippet, read the header), keep `docs/_sidebar.md` in step if a page is added or renamed, and open a pull request that names the owning file for each claim.

| Page | What it answers | Owner of the facts |
|---|---|---|
| [docs/quickstart.md](../../../docs/quickstart.md), [docs/installation.md](../../../docs/installation.md) | first sketch; installing for the Arduino IDE, PlatformIO, by hand | `library.properties`, `library.json`, `examples/01_getConfiguration/01_getConfiguration.ino` |
| [docs/examples.md](../../../docs/examples.md) | what each example sketch shows | `examples/` |
| [docs/api-reference.md](../../../docs/api-reference.md), [docs/doxygen-api.md](../../../docs/doxygen-api.md) | the class, its methods and structs | `LoRa_E220.h` |
| [docs/configuration.md](../../../docs/configuration.md) | every configuration field and value | `includes/statesNaming.h`, `LoRa_E220.h` |
| [docs/transparent-mode.md](../../../docs/transparent-mode.md), [docs/fixed-mode.md](../../../docs/fixed-mode.md), [docs/wor.md](../../../docs/wor.md) | the three ways to transmit, and Wake-on-Radio | `LoRa_E220.cpp`, `examples/` |
| [docs/wiring.md](../../../docs/wiring.md), [docs/platform-setup.md](../../../docs/platform-setup.md), [docs/platform-arduino.md](../../../docs/platform-arduino.md), [docs/platform-esp32.md](../../../docs/platform-esp32.md), [docs/platform-esp8266.md](../../../docs/platform-esp8266.md) | pins, voltage levels, the constructor each board uses | `LoRa_E220.h` (constructor overloads) |
| [docs/troubleshooting.md](../../../docs/troubleshooting.md), [docs/faq.md](../../../docs/faq.md) | symptom to cause | `LoRa_E220.cpp` (response codes from `includes/statesNaming.h`) |
| [docs/building.md](../../../docs/building.md) | building and testing the library | `.github/workflows/build-test.yml`, `platformio.ini` |

## The rest

| Document | What it answers | Owner of the facts |
|---|---|---|
| [README.md](../../../README.md) | what the library is, installing, the quick start, platforms | `library.properties`, `LoRa_E220.h` |
| [CHANGELOG.md](../../../CHANGELOG.md) | what each version changed | the tags and releases ([release.md](release.md)) |
| [CONTRIBUTING.md](../../../CONTRIBUTING.md) | how to send a change | `.github/workflows/build-test.yml` |
| [docs/ARDUINO_LIBRARY_SUBMISSION.md](../../../docs/ARDUINO_LIBRARY_SUBMISSION.md) | listing the library in Arduino Library Manager | `.github/workflows/arduino-submission.yml` |
| [docs/PLATFORMIO_PUBLISHING_GUIDE.md](../../../docs/PLATFORMIO_PUBLISHING_GUIDE.md) | publishing to the PlatformIO registry, by workflow or by hand | `.github/workflows/platformio-publish.yml` |
| [docs/original-documentation.md](../../../docs/original-documentation.md) | the upstream author's documentation of the E220 and this library, kept as written | `LoRa_E220.h` for anything this fork changed |

## Known drift

- **`SECURITY.md`** is a template from another project: it names a different package, a placeholder reporting address and a 1.0.x line. Not pinned until a person rewrites it.
- **`README.md`** links the documentation site at the repository's old name; GitHub Pages serves it under `alteriom-ebyte-lora-e220-library` and does not redirect. It offers installation from Arduino Library Manager, where the library is not listed ([release.md](release.md)). Its "Building and Testing" builds "all platforms" with PlatformIO, but `platformio.ini` defines only the `native` test environment.
- **The manifests** (`library.properties`, `library.json`, `package.json`) name the old repository URL; GitHub redirects it.
- **Duplicates**: `docs/CHANGELOG.md` and `docs/CONTRIBUTING.md` are copies the site shows; the root files are the owners, and `docs/CHANGELOG.md` already lacks the root's "Unreleased" section.
- **`.github/copilot-instructions.md`** said version 1.0.8 and "use library manager"; corrected in this change.
- **Status pages** from the September 2025 setup, superseded by the workflows and [release.md](release.md), not pinned: at the root, ARDUINO_AUTOMATION_SUMMARY, RELEASE_SUMMARY and TRIGGER_RELEASE; under `docs/`, ALL_ISSUES_RESOLVED, CONSTRUCTOR_FIX_PROGRESS, DOCUMENTATION_IMPROVEMENTS, FINAL_PUBLISHING_FIXES, PLATFORMIO_AUTHENTICATION_FIX, PLATFORMIO_SETUP_COMPLETE, QUICK_PLATFORMIO_PUBLISH, YAML_SYNTAX_FIX and DEPLOYMENT_GUIDE (written for v1.0.0); under `.github/`, README-COPILOT, USAGE-EXAMPLE, DOCUMENTATION and RELEASE_TEMPLATE. `.github/mcp-server-config.json` describes tooling no workflow uses, and `.github/copilot-mcp.json` names a server `app.js` this repository does not have.
- **`wiki`** is a submodule entry with no `.gitmodules` behind it, so `git submodule update` cannot fetch it; the wiki itself is written by `update-wiki.yml`.
