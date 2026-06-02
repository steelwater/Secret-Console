# Secret Console

Secret Console is a tiny Arduboy toy where players experiment with button combinations to discover hidden codes, animations, sounds, and surprises.

## Current Prototype

- Starts with `SECRET CONSOLE` and `Press Any Button`
- Shows a blinking terminal cursor
- Displays recent button inputs as `U D L R A B`
- Tracks discovery progress as `Secrets 0/10`
- Detects rolling input-buffer secret codes
- Clears the visible input trail after a secret is found
- Clears inactive input after 3 seconds
- Plays a short reward screen for each discovered secret
- Builds for Arduboy-compatible hardware using `arduino:avr:leonardo`

## Desktop Testing

The repo includes a compiled ProjectABE-ready HEX file:

```text
build/arduboy/SecretConsole.ino.hex
```

Open ProjectABE, then drag that `.hex` file into the browser window:

```text
https://felipemanga.github.io/ProjectABE/
```

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
