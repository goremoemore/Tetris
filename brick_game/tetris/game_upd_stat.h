#ifndef SRC_BRICK_TETRIS_STAT_H
#define SRC_BRICK_TETRIS_STAT_H

#include "tetris_api.h"

void UpdateScore(GameInfo_t *game_info, int count_line_cleared);

void UpdateLevel(GameInfo_t *game_info);

#endif