# 02-release — cut a version and see it into every registry

One run is one version: planned and bumped in a pull request, tagged by a person, its workflows checked registry by registry, and what is still missing handed to a person. No stage pushes a tag, publishes a package, dispatches a publishing workflow or opens a pull request on `arduino/library-registry`: a person does, from what the stage prepared.

Stages, in order; inputs, outputs and checks for each are in [CONTEXT.md](CONTEXT.md):

1. [Plan](stages/01_plan/CONTEXT.md) — the number, what is in it, the bump pull request.
2. [Publish](stages/02_publish/CONTEXT.md) — a person tags; each workflow and registry is checked.
3. [Follow](stages/03_follow/CONTEXT.md) — what is still missing, and who does it.
