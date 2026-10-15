# 02_publish — a person tags, the registries are checked

One job: after a person pushes the tag, check each workflow it started and each registry it should reach.

## Inputs
- Working (this run): `.github/icm/02-release/stages/01_plan/output/plan.md`
- Reference (every run): `.github/icm/_shared/release.md`

Do NOT load: the other pipelines, other runs' output, `.github/icm/_shared/architecture.md`.

## Process
1. The person pushes the tag with the commands in `plan.md`. The agent never runs `git tag`, a push of a tag, `npm publish`, `pio pkg publish`, `gh release create` or a dispatch of a publishing workflow.
2. Watch the tag's runs: `gh run list --workflow release.yml --limit 3` and `gh run list --workflow platformio-publish.yml --limit 3`, then `gh run view <id> --log-failed` for any red job. Name which job failed (tests, release, npm, GitHub Packages, wiki, PlatformIO) and why.
3. Check each registry for the version, the way [release.md](../../../_shared/release.md) "Where each registry stands" reads it: the GitHub release is published (not a draft), npm, GitHub Packages, PlatformIO. Arduino Library Manager only if the library is listed.
4. Record what each registry shows, with the run links.

## Outputs
- publish.md → output/ — the tag; each run and its result; each registry and the version it shows; what is missing and the failing step.

## Human check
A person pushes the tag, then reads `publish.md` and confirms each registry shows the version before anyone is told to use it.
