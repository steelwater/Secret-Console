# Secret Console
### Arduboy Game Brief v0.1

## One Sentence Pitch

A tiny Arduboy toy where players experiment with button combinations to discover hidden secret codes that unlock funny animations, sounds, and surprises.

## Vision

This is not really a game. It is a digital toy.

The enjoyment comes from experimentation, discovery, curiosity, and collection.

## Core Design Principle

Every secret discovered should make the player smile.

## Startup Experience

SECRET CONSOLE

Press Any Button

After any button:

>_

Secrets 0/10

## Main Gameplay Loop

Player presses buttons. Buttons appear on screen. The game continuously stores recent button presses. Whenever the buffer matches a secret code, a reward sequence plays and the secret is permanently unlocked.

## Technical Constraints

- Arduboy
- 128x64 pixels
- Instant startup
- No loading screens
- Small memory footprint
- Single-screen experience
- Playable with one hand

## Captain's Rule

If a new feature reduces curiosity or experimentation: remove it.
