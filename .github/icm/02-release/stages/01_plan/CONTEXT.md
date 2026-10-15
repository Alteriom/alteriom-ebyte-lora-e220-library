# 01_plan — the number, the contents, the bump

One job: a release plan a person can approve, and the pull request that writes the version into the four files and the changelog.

## Inputs
- Working (this run): the changes since the last release — `gh release list --repo Alteriom/alteriom-ebyte-lora-e220-library --limit 5`, then `git log --oneline <last tag>..origin/main`.
- Reference (every run): `.github/icm/_shared/release.md`
- Reference (only when the release changes a public signature or a manifest): `.github/icm/_shared/architecture.md`

Do NOT load: the other pipelines, other runs' output, `.github/icm/_shared/documents.md`, `resources/`, `bin/`.

## Process
1. List the merged changes since the last tag and what each means for a sketch: a fix a user will notice, a new method, a changed default, a platform newly supported.
2. Choose the number by Semantic Versioning, above every version any registry already holds ([release.md](../../../_shared/release.md) "Where each registry stands": read each registry again, they move). Confirm the tag does not exist (`git ls-remote --tags origin`).
3. Read the last tag's `release.yml` and `platformio-publish.yml` runs; if they failed, the plan says what must be fixed on `main` first (a `ci-red` run), before any tag.
4. Prepare the bump on a branch: `scripts/update-version.sh <x.y.z>`, the "Unreleased" section of `CHANGELOG.md` renamed to the version with today's date, `scripts/validate-library.sh` clean. Open it as a pull request after the human check.
5. Write the exact commands the person will run after the bump merges: `git tag v<x.y.z> <merge commit> && git push origin v<x.y.z>`.

## Outputs
- plan.md → output/ — the number and why; each change and what it means for a user; the registries' current versions; the last tag runs and anything to fix first; the bump pull request; the commands for the person.

## Human check
A person approves the number and the bump pull request, and merges it; nothing is tagged until they do.
