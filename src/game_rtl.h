#ifndef GAME_RTL_H
#define GAME_RTL_H

#include "common_cpu_infra.h"

/* One frame of The Simpsons - Barts Nightmare. See game_rtl.c — this is the port. */
void GameRunOneFrame(void);
void GameDrawPpuFrame(void);
void GameSessionReset(void);

extern const RtlGameInfo kGameInfo;

#endif /* GAME_RTL_H */
