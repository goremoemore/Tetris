#ifndef SRC_BRICK_TETRIS_GAME_H
#define SRC_BRICK_TETRIS_GAME_H

#include "game_upd_stat.h"
#include "lines.h"
#include "tetramino_module.h"

void GameProcess(GameInfo_t *game_info, int user_action);

void ManageUserAction(GameInfo_t *game_info, int action);

void ManageGameState(GameInfo_t *game_info);

#endif