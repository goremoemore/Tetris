#ifndef SRC_BRICK_TETRIS_LINES_H
#define SRC_BRICK_TETRIS_LINES_H

#include "tetris_api.h"

void CheckLinesForClear(GameInfo_t *game_info);
void ClearLines(GameInfo_t *game_info, int start_line, int line_count);
void ShiftLines(GameInfo_t *game_info, int start_line, int line_count);

#endif