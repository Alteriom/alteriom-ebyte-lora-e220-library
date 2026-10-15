# 03-deps — the pipeline

One stage: read the bump against what it can break, and say merge, hold or close.

| Stage | Job | Input | Output | Human check |
|---|---|---|---|---|
| [`01_review`](stages/01_review/CONTEXT.md) | what moved, what it can break | the pull request | `output/review.md` | A person reads `review.md` and the pull request's checks, then merges, holds or closes it themselves. |
