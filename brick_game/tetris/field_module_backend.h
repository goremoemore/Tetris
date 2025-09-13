#ifndef SCR_BRICK_TETRIS_FIELD_MD_H
#define SCR_BRICK_TETRIS_FIELD_MD_H

#include "tetris_api.h"

void SetTetraminoToField(GameInfo_t *game_info, int is_ghost);

void DeleteTetraminoFromField(GameInfo_t *game_info, int is_ghost);

#endif