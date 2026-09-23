# Agent Notes

Follow the [Personal Projects Agentic Development Handbook v1.8](https://docs.google.com/document/d/1DwfcO4dqSVc4yr8__NkKWbbNq2othZOgi0t0iDeSHGw/edit) (22 September 2026) alongside these project rules. The current approved Crew Brief defines task scope.

- Verify behavior and Arduboy compilation; follow PR CI checks through completion on the latest commit.
- Run `python3 tests/run-tests.py` for host behavior regression tests.
- Keep generated build outputs out of new commits. Existing tracked binaries are historical; build current source for testing.
- Keep product decisions and handoffs in canonical Google Drive documents; keep setup and technical instructions here.
- PR delivery does not authorize merge, release, deployment, or cleanup.

Secret Console is an Arduboy toy about discovering hidden button codes.

## Standing Context

- Create a `README.md` when a GitHub/code repo does not have one.
- Keep the README updated as code changes.
- Arduboy first.
- Screen resolution is 128 x 64.
- Fun before features.
- Secrets are the game.
- Do not add features without approval.
- When uncertain about scope, stop and ask.

## Design Rules

- Every discovered secret should make the player smile.
- Secrets are optional discoveries, never required for basic interaction.
- Keep startup instant.
- Keep the main loop tiny and readable.
- Preserve curiosity and experimentation.
- Remove features that reduce curiosity or experimentation.

## Coding Standards

- Use 2-space indentation.
- Prioritize readability.
- Document intent, not implementation.
- Avoid unnecessary abstraction.
- Prefer simple data structures.
- Run `scripts\test-arduboy.ps1` before submitting code.

## Local Commands

- Test all sketches: `powershell -ExecutionPolicy Bypass -File .\scripts\test-arduboy.ps1`
- Build prototype: `powershell -ExecutionPolicy Bypass -File .\scripts\build-arduboy.ps1 -Sketch arduboy\SecretConsole\SecretConsole.ino`
- Upload to hardware: `powershell -ExecutionPolicy Bypass -File .\scripts\upload-arduboy.ps1 -Port COMx`
