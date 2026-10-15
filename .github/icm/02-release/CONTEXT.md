# 02-release — the pipeline

Plan the number, let a person tag, check every registry, hand over what is left.

| Stage | Job | Input | Output | Human check |
|---|---|---|---|---|
| [`01_plan`](stages/01_plan/CONTEXT.md) | the number, the contents, the bump | the changes since the last release | `output/plan.md` | A person approves the number and the bump pull request, and merges it; nothing is tagged until they do. |
| [`02_publish`](stages/02_publish/CONTEXT.md) | a person tags, the registries are checked | 01_plan's `plan.md` | `output/publish.md` | A person pushes the tag, then reads `publish.md` and confirms each registry shows the version before anyone is told to use it. |
| [`03_follow`](stages/03_follow/CONTEXT.md) | what is still missing, and who does it | 02_publish's `publish.md` | `output/follow.md` | A person reads `follow.md` and does, or asks for, each step; the release is done when every registry in `publish.md` shows the version. |
