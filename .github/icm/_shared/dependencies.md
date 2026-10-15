---
verified_at: 2026-10-15
verified_commit: 6d202404f514ab13e80a302ead766aca96e29afd
---

# Dependencies — what Dependabot bumps and what each can break

`.github/dependabot.yml`: weekly, two ecosystems. Minor and patch updates are grouped into one pull request per ecosystem; majors come one by one.

| Ecosystem | What moves | What it can break | Proof it did not |
|---|---|---|---|
| `npm` | the dev dependencies of `package.json` (`@eslint/js`, `prettier`); nothing the library or its users run | `npm install` in the npm and GitHub Packages jobs of `release.yml`; no script uses either tool (`package.json` `scripts` only echo) | the diff touches `package.json` and the lock only; the next release's npm jobs |
| `github-actions` | the actions of every workflow in `.github/workflows/` | inputs or defaults of a step: `arduino/setup-arduino-cli` and `arduino/arduino-lint-action` in `build-test.yml`; `upload-pages-artifact`, `configure-pages`, `deploy-pages` in `documentation.yml`; `softprops/action-gh-release` in `release.yml` | the pull request's own `Build and Test Library` run; a bump to a release, publishing or Pages action is not exercised by a pull request, so read its changelog for removed inputs |
| the actions of `icm-check.yml` | pinned by commit and vendored unchanged from the ICM standard | — | close it; take the standard's next version instead |

The Arduino cores and `arduino-cli` 1.0.4 are pinned in `build-test.yml` itself, and Unity in `platformio.ini`; Dependabot does not move them.
