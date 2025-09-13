#ifndef SRC_BRICK_TETRIS_TIMER_H
#define SRC_BRICK_TETRIS_TIMER_H

#include "tetris_api.h"

int ShouldDropPiece(GameInfo_t *game_info, int level_time);

void ResumeGameForTimer(GameInfo_t *game_info);

void PauseGameForTimer(GameInfo_t *game_info);

clock_t getAdjustedTime(GameInfo_t *game_info);

#endif