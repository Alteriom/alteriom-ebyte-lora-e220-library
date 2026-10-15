# 03_follow — what is still missing, and who does it

One job: for each registry or record that does not show the version yet, the step that fixes it, prepared for a person.

## Inputs
- Working (this run): `.github/icm/02-release/stages/02_publish/output/publish.md`
- Reference (every run): `.github/icm/_shared/release.md`
- Reference (only when the release changed what a page of the site installs or shows): `.github/icm/_shared/documents.md`

Do NOT load: the other pipelines, other runs' output.

## Process
1. A registry the workflow missed: the command a person runs (a re-run of the failed job, a dispatch of `platformio-publish.yml`, a manual publish from `docs/PLATFORMIO_PUBLISHING_GUIDE.md`), or a fix on `main` first if the workflow is broken. Never the same version twice on a registry that already has it.
2. Arduino Library Manager: if `arduino-submission.yml` opened its issue, the pull request to `arduino/library-registry` a person opens ([release.md](../../../_shared/release.md)); if the library is listed, check that the indexer took the tag.
3. The site and `README.md`: if they name a version or an install line that changed, prepare the docs pull request here ([documents.md](../../../_shared/documents.md)).
4. Close the run: the draft releases or stale tags it left, said to the person, not deleted by the agent.

## Outputs
- follow.md → output/ — per registry or record: what is missing, the step that fixes it, and who does it.

## Human check
A person reads `follow.md` and does, or asks for, each step; the release is done when every registry in `publish.md` shows the version.
