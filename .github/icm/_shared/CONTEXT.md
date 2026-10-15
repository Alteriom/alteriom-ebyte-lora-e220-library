# Shared reference

What every job on this repository draws on. Each file points at the repository file that owns a fact; follow the pointer for the detail, and when the two disagree the owning file is right.

| File | Answers |
|---|---|
| [architecture.md](architecture.md) | where each question is answered in the library, and what is easy to break (constructors per platform, include names, the version) |
| [build-and-test.md](build-and-test.md) | compiling and testing what CI does, what each workflow runs and when, what counts as evidence |
| [release.md](release.md) | the version files, what a tag starts, where each registry stands, what is easy to get wrong |
| [dependencies.md](dependencies.md) | what Dependabot bumps and what each can break |
| [documents.md](documents.md) | the documentation site and every document, who owns its facts, and the known drift |

Each stage's contract names the files it loads.
