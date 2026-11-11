# Changelog

## 0.1.2 — 2026-11-11

Current snesrecomp and recomp-ui, and RecompLauncher's recommendations. The
framework brings its access-exact sound timing (a per-clock S-DSP), sprite
priority, brightness-rounding, HDMA and mosaic fixes: the intro's fades now
match the console where 0.1.1 was a level off. Save states from 0.1.1 keep
loading, at the doors or inside a nightmare, with the music playing on (the
old sound state is converted). With [RecompLauncher](https://recomplauncher.com)
the game builds on the launcher's setup screen, Play goes straight into the
game, Big Picture opens full screen, and the renderer and shader are on the
game's page. Controllers are no longer read while the window is in the
background ([General] BackgroundInput = 1 restores that); the log keeps the
last runs.

## 0.1.1 — 2026-11-11

RecompLauncher support: the release carries a `recomplauncher.json`, so
[RecompLauncher](https://recomplauncher.com) can install the game, ask for your
ROM, open the setup with it already picked, and show the window, picture and
sound settings on the game's page. Nothing changes in the game itself; saves
and save states from 0.1.0 keep working.

## 0.1.0 — 2026-11-11

First release: a setup pack that recompiles the game from your own ROM. Boots
through the credits, the Acclaim logo and the title screen into Springfield's
streets, at the console's speed; checked against snes9x with the same input
(same pictures, same pace). The cartridge has no battery; save states are
kept in `build\saves\`. A fresh install starts with no shader (the plain
picture); scanline, CRT and upscaler presets (ScaleFX, NNEDI3) are in Settings
→ Shader. Settings during play (Ctrl+L or Select+L3), and a setting that
needs a restart comes back to the same moment; audio interpolation (I key:
Gaussian, Cubic, Sinc); update check on start that backs up your saves before
installing; asks for the dedicated graphics card on laptops with two;
console-free Windows build.

All six nightmares were entered from the doors, played and left back to the
street. Fixed on the way: the doors and the nightmares read the pad by hand and
saw every button held (a door was chosen by itself, and a nightmare dropped into
its pause screen and stayed frozen with the music playing); a save state loaded
in another scene came back garbled; save states now keep the CPU's place, so a
state made at the doors or in a nightmare resumes there.

It ships with every framework fix of the earlier ports (see README → Framework
patches), so the problems found there do not come back here.
