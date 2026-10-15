# 01-fix — the pipeline

Make it fail, make it pass for the right reason, prove it on every platform the change touches.

| Stage | Job | Input | Output | Human check |
|---|---|---|---|---|
| [`01_reproduce`](stages/01_reproduce/CONTEXT.md) | reproduce the reported defect | the report | `output/reproduction.md` | A person runs the command, or reads the output and the code path, and confirms it fails for the reported reason and not a neighbouring one. |
| [`02_fix`](stages/02_fix/CONTEXT.md) | change the cause, not the symptom | 01_reproduce's `reproduction.md` | `output/change.md` | A person reads the diff beside the reproduction: it removes the cause, keeps existing sketches compiling, and changes no public signature. Edit `change.md` if the explanation is wrong; stage 03 reads it. |
| [`03_verify`](stages/03_verify/CONTEXT.md) | checks, records, pull request | 02_fix's `change.md` | `output/pull-request.md` | A person reads `pull-request.md` and its evidence and says whether to open it; the reviewer, not the author, merges. |
