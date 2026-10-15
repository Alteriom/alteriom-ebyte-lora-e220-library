---
verified_at: 2026-10-15
verified_commit: b97a85eaa226bdf703382477db5769737fb0f14f
---

# Releasing — a person tags, the workflows publish

## The version

Four files carry it and must agree: `library.properties` (`version=`), `library.json`, `package.json` and the header comment of `LoRa_E220.h` (`VERSION:`). `scripts/update-version.sh <x.y.z>` rewrites all four; `CHANGELOG.md` gets its section by hand (Keep a Changelog, Semantic Versioning, as its header says). The tag is `v<x.y.z>` on the commit that carries that version.

## What a tag starts

A `v*` tag pushed by a person starts two workflows:

- `.github/workflows/release.yml`: runs `build-test.yml` first, then creates the GitHub release with generated notes, publishes to npm as the unscoped `alteriom-ebyte-lora-e220` (repository secret `NPM_TOKEN`) and to GitHub Packages as `@alteriom/alteriom-ebyte-lora-e220`, and updates the repository wiki.
- `.github/workflows/platformio-publish.yml`: validates `library.json`, compiles, looks the library up in the PlatformIO registry and publishes as owner `alteriom` (repository secret `PLATFORMIO_AUTH_TOKEN`) only when the registry's version differs; a dispatch with `force_publish` overrides that.

A published release starts `.github/workflows/arduino-submission.yml`, which checks whether the repository is listed in the Arduino library registry and, if not, opens a submission issue here. Being listed takes a pull request to `arduino/library-registry` that a person opens (`docs/ARDUINO_LIBRARY_SUBMISSION.md`); once listed, Library Manager indexes each new tag by itself.

## Where each registry stands (read 2026-10-15)

| Registry | Latest | Read with |
|---|---|---|
| GitHub releases | v1.1.5; a v1.1.6 release is a draft and the tag `v1.1.6` no longer exists | `gh release list --repo Alteriom/alteriom-ebyte-lora-e220-library` |
| PlatformIO | 1.1.6, published 2025-09-30 outside the workflow: every run of `platformio-publish.yml` failed | `https://api.registry.platformio.org/v3/search?query=Alteriom_EByte_LoRa_E220` |
| npm (unscoped) | 1.1.5 | `npm view alteriom-ebyte-lora-e220 version` |
| GitHub Packages (scoped) | needs a token to read | the repository's Packages page |
| Arduino Library Manager | not listed | `repositories.txt` of `arduino/library-registry` |

So the next release is 1.1.7 or later: PlatformIO already has 1.1.6, and a registry never takes the same version twice. The manifests in the tree say 1.1.6.

## Things that are easy to get wrong

- The repository was renamed from `Alteriom/EByte_LoRa_E220_Series_Library`. The manifests' `url`, `repository` and `homepage`, the Pages links in `README.md` and the registry search in `arduino-submission.yml` name the current one; GitHub redirects the old name, Pages does not, so a link to the old Pages address is a 404.
- The scoped name in `package.json` is the GitHub Packages one (`.npmrc` points `@alteriom` at GitHub's registry); `release.yml` rewrites it to the unscoped name only for the npm step and restores it after.
- Every tag between v1.1.1 and v1.1.6 except v1.1.5 failed `release.yml` (the run list of that workflow); read the log of the last tag run before assuming the workflow still works, and fix it on `main` before tagging (the `ci-red` route).
