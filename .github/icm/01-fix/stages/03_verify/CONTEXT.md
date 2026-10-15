# 03_verify — checks, records, pull request

One job: the pull request a reviewer can merge, with the evidence CI will look for and the records brought along.

## Inputs
- Working (this run): `.github/icm/01-fix/stages/02_fix/output/change.md`
- Reference (every run): `.github/icm/_shared/build-and-test.md`
- Reference (only when the fix changes what a page of the site or `README.md` says): `.github/icm/_shared/documents.md`

Do NOT load: the other pipelines, other runs' output, `.github/icm/_shared/release.md`.

## Process
1. Run what CI runs ([build-and-test.md](../../../_shared/build-and-test.md)): the compile matrix for every sketch touched, `pio test -e native`, `arduino-lint` strict.
2. Record: add the fix under "Unreleased" in `CHANGELOG.md`; correct a page under `docs/` or `README.md` that described the wrong behaviour; correct `.github/icm/_shared/` if a pointer moved. Do not bump the version: that is the release job.
3. Write the pull request: what was wrong, how you know (the reproduction), what the change does, the boards compiled, whether it ran on a module, and `Fixes #<n>`.
4. After the human check, open it against `main` (`gh pr create --base main --body-file <this stage's output/pull-request.md>`). The agent never merges.

## Outputs
- pull-request.md → output/ — the title and body; the checks run with results; the records updated; the module run or why there is none.

## Human check
A person reads `pull-request.md` and its evidence and says whether to open it; the reviewer, not the author, merges.
