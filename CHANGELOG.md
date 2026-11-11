# Changelog

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
