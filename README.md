# The Simpsons: Bart's Nightmare Recomp

A native recompilation of **The Simpsons: Bart's Nightmare** (USA), built on
[snesrecomp](https://github.com/RetroPortingToolKit/snesrecomp).

Bart's Nightmare is Sculptured Software's 1992 Simpsons game for the Super
NES, published by Acclaim: Bart falls asleep over his homework, and the
pages blow away into his dreams. He chases them down Springfield's streets
and into mini-games inside his nightmares: Bartzilla, the Itchy & Scratchy
house, Bart's bloodstream and the temple of Indiana Bart.

> **You must legally own a copy of the game.** No ROM data is distributed with
> this project, in the repository or in any release. The recompiled C is
> generated locally from your own copy and is never committed.

## ☕ Keep the ports coming

<p align="center">
  <a href="https://buymeacoffee.com/mpolizel"><img src="https://img.shields.io/badge/Buy%20me%20a%20coffee-support%20more%20ports-FFDD00?style=for-the-badge&logo=buymeacoffee&logoColor=black" alt="Buy me a coffee"></a>
</p>

**This project is free and always will be.** Every port in this series,
Bart's Nightmare included, is built with many hours of AI-assisted reverse engineering,
and that AI time is paid. If you enjoy playing it, a coffee at
**[buymeacoffee.com/mpolizel](https://buymeacoffee.com/mpolizel)** keeps the AI running so **more SNES
games can be ported to native Recomp**. Any amount helps, and thank you!

## Download (ready-to-run Windows build)

Get the latest `bartsnightmare-<version>-windows-x64.zip` from the
[Releases page](https://github.com/mpolizel/TSBNSNESRecomp/releases/latest).

The zip is a **setup pack**: it contains the launcher, the recompiler and this
source tree (with the framework patches already applied), but no game data.
On first run:

1. Unzip anywhere and start `TheSimpsonsBartsNightmareSNESRecomp.exe`.
2. Let it download the portable build tools (one time per machine), or point
   it at a `cmake-clang-v1` zip you already have.
3. Pick your own `Simpsons, The - Bart's Nightmare (USA).sfc` (CRC32 `ed1c03c2`;
   the file name does not matter, the contents are checked).
4. Press **Generate & rebuild**. The playable game is built into `build/` and
   started; run it from there afterwards.

In the launcher, **Settings → Shader** lists the scanline, CRT and upscaler
presets; choosing one switches the renderer to OpenGL automatically. A fresh
install starts with no shader (the plain picture), the lightest option for
any graphics card; pick a CRT or upscaler preset there if yours has room.
Your choice is saved in `config.ini` and kept. The upscalers include
**ScaleFX** and the neural **NNEDI3** presets.

During play, **Ctrl+L** (or **Select+L3** on a controller) reopens the
launcher: shader, window scale, volume and controls apply when you press
**Resume**, without restarting the game. **I** cycles the audio interpolation
between **Gaussian** (the console's own, the default), **Cubic** (crisper
highs) and **Sinc**; it only changes what you hear, never the game, and the
choice is saved in `audio_interpolation.txt`.

On laptops with two graphics cards (NVIDIA Optimus, AMD switchable graphics)
the game asks the driver for the dedicated one. If it still runs slowly,
set the game's executable to **High performance** in Windows Settings →
System → Display → Graphics.

### Saves and updates

The cartridge has no battery. Save states (Settings → Save states, or the save-state
hotkeys) are kept in `build\saves\`.

With **Check for updates on start** ticked (the default; on the launcher's main
screen, next to **Skip launcher on boot**), the game asks GitHub for the latest
release when it starts and offers to install a newer one. Accepting closes the
game, downloads the new setup pack, copies it over the install folder and
reopens the setup window: press **Generate & rebuild**. Nothing is deleted;
`build\` (the rebuilt game, `saves\`, save states, `config.ini`,
`keybinds.ini`) and `rom.cfg` are kept. Untick the box to turn the check off
(saved in `update.ini`).

**Your saves are never lost or broken by an update.** In-game saves and save
states made by any release keep working in every later one. Before installing,
the updater copies `build\saves\` to `save-backups\<date>_before_<version>\`
and never deletes those copies; save files are left out of the copy, and if
they changed anyway the backup is put back. Every release after the first is
checked against the one before it with `tools/check_update_compat.ps1`, which
builds the previous release, makes an in-game save and a save state with it,
installs the new pack with the previous release's own updater and rebuilds; it
fails unless every save file is byte-identical and the new build continues the
old save state.

See [CHANGELOG.md](CHANGELOG.md) for what changed in each release.

## Status

Boots through the credits, the Acclaim logo and the title screen into
Springfield's streets, at the console's speed. Checked against snes9x with the
same input: the attract pictures match exactly, the street plays the same,
and save states and setting changes during play resume where they were. All
six nightmares (Bartzilla, Bartman, Bart's Bloodstream, the Temple of Maggie,
Itchy & Scratchy and the Windy building) were entered from the doors, played
with the pad and left back to the street. Most code still runs through the
snesrecomp interpreter. See [Porting from here](#porting-from-here).

### Framework patches

`patches/` holds changes to the `snesrecomp` and `recomp-ui` submodules.
Reapply them after updating the submodules:

```sh
git -C snesrecomp apply ../patches/snesrecomp-apu-first-read-timeline.patch
git -C snesrecomp apply ../patches/snesrecomp-audio-interpolation.patch
git -C snesrecomp apply ../patches/snesrecomp-glsl-passprev-and-vao.patch
git -C snesrecomp apply ../patches/snesrecomp-joypad-manual-strobe.patch
git -C snesrecomp apply ../patches/snesrecomp-mosaic-frame-model.patch
git -C snesrecomp apply ../patches/snesrecomp-nmi-idle-loop-wai.patch
git -C snesrecomp apply ../patches/snesrecomp-nmi-wai-in-handler.patch
git -C snesrecomp apply ../patches/snesrecomp-raster-oam-and-hdma.patch
git -C snesrecomp apply ../patches/snesrecomp-raster-vram-journal.patch
git -C snesrecomp apply ../patches/snesrecomp-resume-state-restart.patch
git -C snesrecomp apply ../patches/snesrecomp-shader-forces-opengl.patch
git -C snesrecomp apply ../patches/snesrecomp-state-load-raster-reset.patch
git -C snesrecomp apply ../patches/snesrecomp-timing-frame-pacing.patch
git -C recomp-ui apply ../patches/recomp-ui-host-toggle.patch
```

`snesrecomp-apu-first-read-timeline.patch` lets the sound chip run from power
on when the game reads its ports before writing them. Kirby Super Star read
the sound port once, right after the SA-1 finishes its start-up, and uploads
its sound driver only if the chip's boot ROM answers; the framework started
the sound chip's clock at the first port write, so the read found nothing and
the game waited forever on a black screen.

`snesrecomp-raster-oam-and-hdma.patch` is the renderer fix shared with the
other ports: per-line sprite tables, HDMA set up in vblank, and vblank writes
kept in their own field. It also counts HDMA's first transfer, made in V=0's
H-blank, as a table line, as the hardware does. 
`snesrecomp-raster-vram-journal.patch` makes the renderer draw each line with
the video memory, palettes and sprite table it had on hardware. The renderer
runs after the CPU half of a frame, so a write made while the picture is
drawn (in forced blank at the bottom of the screen) used to reach every line
of that frame; it is now journaled and replayed from its own line, and writes
made by the next frame's NMI before the render are kept out. It also reads
BG mode 2's offset-per-tile rows at BG3VOFS and BG3VOFS+8 (the code read
BG3VOFS-1 and +7, a row off when BG3VOFS is a multiple of 8) and drops a +1
on the vertical offset, as bsnes does. Both showed in Yoshi's Island's World 1-7: the stage
was assembled from the wrong rows, then shook 8 pixels back and forth while
Yoshi walked, because the next frame's offset table, uploaded at line 217,
was drawn under this frame's fine scroll. With video memory journaled, the
per-line sprite table of `snesrecomp-raster-oam-and-hdma.patch` applies to
every frame that rewrites it again.

`snesrecomp-resume-state-restart.patch` makes a setting that needs a restart
come back to the same moment. The in-game launcher saves `saves\resume.sav`,
restarts and loads it before the first frame, when no session exists yet. A
save state holds the machine but not the CPU half's registers or resume
point (a load during play keeps the session's own, the game's idle loop), so
the frame driver booted from the reset vector over it. The restart file is
now written in the rollback format (machine plus execution residue), and the
frame driver takes the interpreter's resume point at the start of a frame.
Save-state slots keep their format.

`snesrecomp-joypad-manual-strobe.patch` passes the manual-read strobe
($4016 write) to the controller. The reads already reached it, the strobe did
not, so a game that reads the pad by hand got its sixteen buttons once and then
every button held, for good. Bart's Nightmare reads the pad that way in the
doors and the nightmares: it chose a door by itself and, inside a nightmare,
fell into its pause screen and waited for a Start press that could never come
(the picture frozen, the music still playing).

`snesrecomp-state-load-raster-reset.patch` re-arms the raster journal when a
save state loads. The journal keeps the register file as the last field
started and puts it back before drawing, so after a load the previous scene's
BG mode, tilemaps and windows replaced the state's; a scene that sets them only
once (Bart's Nightmare's title, its doors) stayed garbled until it changed.

This project also saves the CPU's place in each save state
(`src/game_rtl.c`): the game has a main loop per mode (street, doors, each
nightmare), and a load resumed in the session's own loop. A state from an
earlier build still loads as before.

`snesrecomp-timing-frame-pacing.patch` keeps the game at the console's 60.1
frames per second. A frame whose CPU half runs past the end of the field is
followed by a shorter one, and the host counted the overrun twice (the short
frame was raised to a whole one); it also waited past its deadlines and threw
the lateness away. Flashback's polygon intro, which overruns often, ran at
58.9 fps and fell behind the console; now 59.9, with the same pictures.

`snesrecomp-mosaic-frame-model.patch` builds the mosaic table on every visible
line. The optimized renderer built it only on line 0, which frame-model hosts
never render, so every line of a mosaic-enabled layer sampled the layer's
first row.

`snesrecomp-nmi-idle-loop-wai.patch` and `snesrecomp-nmi-wai-in-handler.patch`
make a `WAI` halt until the next interrupt, as on hardware, in the main loop
and inside an interrupt handler; without them the interpreter could spin to
its step cap and freeze the screen.

`snesrecomp-audio-interpolation.patch` adds the Cubic and Sinc audio
interpolation (I key). They resample the dry output only; the echo buffer,
OUTX, pitch modulation and save states stay those of the console's Gaussian
filter, so the game runs identically in every mode.

`snesrecomp-glsl-passprev-and-vao.patch` fixes two shader bugs: a pass that
reads `PassPrev<n>` back to the game frame got nothing (ScaleFX drew black),
and switching to no shader mid-game drew a black screen because the shader
path left the plain path's vertex attributes disabled.

`snesrecomp-shader-forces-opengl.patch` switches the renderer to OpenGL when a
shader is picked, since the presets only run there.

`recomp-ui-host-toggle.patch` lets a title add one checkbox of its own beside
"Skip launcher on boot"; this project uses it for the update check.

## ROM identity

| | |
|---|---|
| File | `Simpsons, The - Bart's Nightmare (USA).sfc` (the apostrophe-free name works too) |
| Publisher | Acclaim |
| Developer | Sculptured Software |
| Year | 1992 |
| Mapping | lorom |
| Region | USA (North America) |
| Coprocessor | none |
| CRC32 | `ed1c03c2` |
| SHA-256 | `f0d98e9061d0f6a193bb856de8a592f336dada97c41966e8d03119ba97465413` |

`tools/regen.sh` refuses to run against anything else, so a mismatched
revision fails immediately instead of producing subtly wrong output.

## Build

```sh
git submodule update --init --recursive
bash tools/regen.sh --rom "/path/to/Simpsons, The - Barts Nightmare (USA).sfc"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

The ROM does not have to live in the repository — keeping it on your own
drive is the better habit, and `SNESRECOMP_ROM` sets the path once for a
shell. Without either, `tools/regen.sh` finds
`Simpsons, The - Barts Nightmare (USA).sfc` at the repo root, then the path
the setup wizard recorded in `rom.cfg`; `.gitignore` blocks both from ever
being committed.

`tools/regen.sh` verifies the ROM, generates `src/gen/*.c`, and re-syncs
`recomp/funcs.h`. Re-run it whenever you change anything under `recomp/`.

The Windows build is a window app: diagnostics go to
`TheSimpsonsBartsNightmareSNESRecomp.log` beside the executable.
`-DTSBN_CONSOLE=ON` brings the console back for debugging.

## Run

```sh
./build/TheSimpsonsBartsNightmareSNESRecomp                    # launcher picks the ROM
./build/TheSimpsonsBartsNightmareSNESRecomp "/path/to/Simpsons, The - Barts Nightmare (USA).sfc"
```

With no ROM on the command line the build opens the recomp-ui launcher: a ROM
picker with a verification badge, plus display / audio / input settings. Your
choice is cached in `rom.cfg` beside the executable, so the next launch opens
on it and "Skip launcher on boot" makes it immediate.

Either way the ROM is checked against the digests above before anything boots
— a dump that could not have produced this build is refused at the door rather
than mis-executing ten frames in.

## Layout

| Path | What lives there |
|---|---|
| `recomp/` | Analysis input: `bank*.cfg`, `symbols.toml`, generated `funcs.h` |
| `rom_identity.txt` | ROM digests — read by the build, `tools/regen.sh` and CI |
| `src/` | Host code you own: `main.c`, `game_rtl.c`, `tsbn_update.c` |
| `src/gen/` | Generated C. Never committed — regenerate locally |
| `assets/shaders/` | Scanline, CRT and upscaler presets for the launcher |
| `patches/` | Changes to the framework submodules (see above) |
| `snesrecomp/` | Framework submodule (owns `lib/recomp-net`, `lib/retcomm-rbengine`) |
| `tools/` | `regen.sh` — the ROM → C pipeline; `check_update_compat.ps1`; `import_libretro_shaders.py` |
| `scripts/` | `package_release.sh` — player-facing zip |
| `framework_pins.txt` | Exact framework commits this project was scaffolded against |

## Porting from here

The scaffold stops where the game-specific work starts. In rough order:

1. **Make it boot.** `src/game_rtl.c` holds the frame driver. It starts on
   the framework's beam-aligned driver (`snesrecomp/runner/src/beam_frame_driver.h`):
   one frame per PPU field, interrupts taken where the beam latches them,
   the field rasterized from the raster journal.
2. **Name things.** Add entries to `recomp/symbols.toml` as you identify
   routines, then re-run `tools/regen.sh`. Set `emit = true` to promote one
   into ahead-of-time analysis; leave it false to keep it interpreted.
   See `recomp/README.md`.
3. **Resolve dispatch misses.** After every run, deal with unresolved
   indirect targets before anything else — they are the reason a port
   diverges, and they are cheap to fix early.
4. **Never synthesise a result** to get past uncovered code, and never edit
   `src/gen/` by hand. Fix the config or the framework and regenerate.

## Multiplayer

Single player.

## License

This project's own source is under the MIT license in `LICENSE`. The
framework carries its own terms (PolyForm Noncommercial 1.0.0) — see
`snesrecomp/LICENSE` and `snesrecomp/THIRD_PARTY_ATTRIBUTION.md`; the changes
in `patches/` apply to that framework and fall under its terms. The shaders
under `assets/shaders/_libretro/` come from libretro/glsl-shaders and keep each
author's license in their file headers. None of these cover the game data,
which is not distributed here.

---

<p align="center">
  Enjoying Bart's Nightmare Recomp? <a href="https://buymeacoffee.com/mpolizel"><b>☕ Buy me a coffee</b></a> to help
  bring the next SNES game to native Recomp.
</p>
