# 01-fix — fix a defect

One run is one defect in the library, an example or a workflow: reproduced, fixed at its cause without breaking an existing sketch, and proposed as a pull request with the evidence. A fix reaches users only through a release (`02-release`).

Stages, in order; inputs, outputs and checks for each are in [CONTEXT.md](CONTEXT.md):

1. [Reproduce](stages/01_reproduce/CONTEXT.md) — make it fail on purpose.
2. [Fix](stages/02_fix/CONTEXT.md) — change the cause, watch the reproduction pass.
3. [Verify](stages/03_verify/CONTEXT.md) — compile what CI compiles, update the records, the pull request.
