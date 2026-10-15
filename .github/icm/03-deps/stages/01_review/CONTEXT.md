# 01_review — what moved, what it can break

One job: a merge, hold or close recommendation for one Dependabot pull request, with the evidence.

## Inputs
- Working (this run): the pull request — `gh pr view <n> --comments`, `gh pr diff <n>`, `gh pr checks <n>`.
- Reference (every run): `.github/icm/_shared/dependencies.md`

Do NOT load: the other pipelines, other runs' output, `.github/icm/_shared/documents.md`, `resources/`, `bin/`.

## Process
1. Name what moved and from which version to which; for a grouped pull request, every package in it. Read the release notes of a major.
2. Look up what it can break in [dependencies.md](../../../_shared/dependencies.md). For an action used only by a release, publishing or Pages workflow, the pull request's run does not exercise it: read its changelog for removed or renamed inputs and compare with how the workflow calls it.
3. If the bump touches `.github/workflows/icm-check.yml`, recommend closing it: that file is vendored from the ICM standard.
4. Recommend merge, hold (with what would unblock it) or close (with why). Never merge.

## Outputs
- review.md → output/ — what moved; what it can break and where that was checked; the CI result; the recommendation and its reason.

## Human check
A person reads `review.md` and the pull request's checks, then merges, holds or closes it themselves.
