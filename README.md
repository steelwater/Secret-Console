# Secret Console

Secret Console is a tiny Arduboy toy where players experiment with button combinations to discover hidden codes, animations, sounds, and surprises.

## Current Prototype

- Opens with a simple `PLAY` / `FOUND SECRETS` / `ABOUT` menu
- Explains the goal and controls across four spoiler-free About pages
- Shows a blinking terminal cursor
- Displays recent button inputs as `U D L R A B`
- Tracks discovery progress as `Secrets 0/10`
- Detects rolling input-buffer secret codes
- Clears the visible input trail after a secret is found
- Shows a small countdown before inactive input clears after 3 seconds
- Replays every matched secret without increasing unique discovery progress
- Lists only discovered combinations, with optional reward replay
- Plays a short reward screen for each discovered secret
- Plays small unique reward jingles
- Builds for Arduboy-compatible hardware using `arduino:avr:leonardo`

## Desktop Testing

Build the current source first. Historical tracked binaries do not reflect new source changes. The build produces a ProjectABE-ready HEX file:

```text
build/arduboy/SecretConsole.ino.hex
```

Open ProjectABE, then drag that `.hex` file into the browser window:

```text
https://felipemanga.github.io/ProjectABE/
```

## Found Secrets Controls

- Main menu: UP/DOWN selects, A opens.
- PLAY: tap buttons to enter codes; hold B for one second to return to the menu (also works during rewards).
- FOUND SECRETS: UP/DOWN browses discovered entries, A replays, B returns to the menu. Rewards return to the selected entry after three seconds.
- Discoveries and the gallery last for the current powered-on session, matching the original prototype. Restarting clears progress.
- Re-entering a discovered code always replays its reward; only first discoveries increase `Secrets X/10`.
- When a short code is embedded in a longer code being entered, its reward waits for the next input or the existing three-second idle timeout so the longer secret remains discoverable.

## Secrets

The first prototype includes the brief's initial reward set:

- Moon Cat
- Rocket Launch
- Tiny UFO
- Dancing Robot
- Fake Crash
- Screen Explosion
- Cursor Escape
- Secret Developer Room
- Huni Robot
- Final Secret

The final secret is intentionally not listed here.

## Development

Set up or refresh the local Arduboy toolchain:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\setup-arduboy.ps1
```

Compile all Arduboy sketches:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\test-arduboy.ps1
```

Build the prototype:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\build-arduboy.ps1 -Sketch arduboy\SecretConsole\SecretConsole.ino
```

Upload to an Arduboy-compatible device:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\upload-arduboy.ps1 -Port COMx
```

## Design Constraints

- Target: Arduboy
- Screen: 128 x 64 pixels
- Single-screen experience
- Instant startup
- Small memory footprint
- Playable with one hand

Captain's rule: if a feature reduces curiosity or experimentation, remove it.

## Verification and Repository Workflow

Follow the [Personal Projects Agentic Development Handbook v1.8](https://docs.google.com/document/d/1DwfcO4dqSVc4yr8__NkKWbbNq2othZOgi0t0iDeSHGw/edit).

Run host behavior regression tests with Python 3 and a C++ compiler:

```sh
python3 tests/run-tests.py
```

CI runs those tests and the existing Windows Arduboy setup/test scripts. Both jobs must pass on the latest PR commit. No separate lint or type-check tooling is configured for this Arduino sketch. ProjectABE and hardware playtesting remain separate checks; host tests use display/audio doubles and cannot prove device presentation or audio quality.

On macOS/Linux, an installed Arduino CLI can compile with the same board and libraries (`arduino:avr`, `Arduboy2`, `ArduboyTones`):

```sh
arduino-cli compile --fqbn arduino:avr:leonardo --output-dir /tmp/secret-console-build arduboy/SecretConsole
```

Keep generated artifacts out of source commits. Existing tracked binaries are retained as historical files; removing them requires a separately approved cleanup. Approved release artifacts belong in GitHub Releases.
