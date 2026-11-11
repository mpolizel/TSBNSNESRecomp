/*
 * The Simpsons - Barts Nightmare — per-game runtime glue.
 *
 * THIS FILE IS THE PORT. Everything else the scaffold produced is layout,
 * build wiring, and packaging; this is where the actual work happens.
 *
 * The framework does not, and cannot, drive an arbitrary SNES game on its
 * own. Two responsibilities always land on the game host:
 *
 *   1. Deciding what "one frame" means for THIS title, and returning from
 *      run_frame() at that boundary. A real ROM's reset vector never
 *      returns — it enters a main loop that waits on vblank — so somebody
 *      has to choose the yield point.
 *   2. Delivering NMI/IRQ at the hardware edge (see
 *      snesrecomp/docs/FRAME_MODEL_HOSTS.md).
 *
 * A new port starts on the framework's beam-aligned driver
 * (runner/src/beam_frame_driver.h): one frame is one PPU field, cut at the
 * beam's vblank edge; interrupts -- raster splits and a coprocessor's IRQ
 * line -- are taken where the beam latches them; parked time still runs the
 * beam, APU and coprocessors; and the field is rasterized from the raster
 * journal with real HDMA. It is the framework's, so a fix to it reaches this
 * port on a submodule pull.
 *
 * This file used to hold its own copy of a simpler driver: frames measured
 * from wherever the CPU last stopped, NMI delivered at an arbitrary beam
 * position, the field drawn with end-of-frame register state. A game that
 * uploads in vblank between INIDISP=$80 and $0F flickered black, and one
 * that chains raster splits fell apart. Replace these calls only for what
 * the driver does not do; MetalWarriorsSNESRecomp's src/mw_rtl.c and
 * GundamWingEndlessDuelSNESRecomp's src/game_rtl.c are developed examples of
 * title-specific drivers.
 */

#include "game_rtl.h"

#include "beam_frame_driver.h"
#include "common_rtl.h"

/* Save states carry the CPU's place too, not only memory.
 *
 * This game has a main loop per mode (the street, the two doors, each
 * nightmare), and the guest blob alone says nothing about which one the CPU
 * was in. A state loaded in a running session resumed in that session's
 * loop: saved at the doors and loaded from the street, the street's loop ran
 * over the doors' memory and the picture fell apart. The framework's
 * execution chunk (CPU registers, the interpreter's resume point, beam and
 * APU pacing, the same residue rewind uses) is appended after the guest
 * blob; a state without it still loads as before. */
static void TsbnStateSaveExtra(struct SaveLoadInfo *sli)
{
    RtlSaveExecutionState(sli);
}

static bool s_tsbn_state_has_execution;

static void TsbnStateLoadExtra(struct SaveLoadInfo *sli, uint32_t version)
{
    (void)version;
    s_tsbn_state_has_execution = RtlLoadExecutionState(sli);
}

static void TsbnStateLoaded(uint32_t version)
{
    (void)version;
    if (s_tsbn_state_has_execution)
        RtlApplyExecutionState();
    s_tsbn_state_has_execution = false;
}

void GameRunOneFrame(void)
{
    snes_beam_frame_driver_run_frame();
}

void GameDrawPpuFrame(void)
{
    snes_beam_frame_driver_draw_ppu_frame();
}

void GameSessionReset(void)
{
    /* Rematch / soft-return: clear anything that must not survive a new
     * session. recomp-ai-rules/NETPLAY.md §3 — sticky state that "has always
     * been fine" is the usual desync culprit, because single-player never
     * re-enters the boot path twice in one process. */
    snes_beam_frame_driver_reset();
}

const RtlGameInfo kGameInfo = {
    .title = "thesimpsonsbartsnightmare",
    .initialize = NULL,
    .run_frame = &GameRunOneFrame,
    .draw_ppu_frame = &GameDrawPpuFrame,
    .save_name_prefix = "save",
    .session_reset = &GameSessionReset,
    .state_save_extra = &TsbnStateSaveExtra,
    .state_load_extra = &TsbnStateLoadExtra,
    .on_state_loaded = &TsbnStateLoaded,
};
