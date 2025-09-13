#ifndef SCR_BRICK_TETRIS_TETRA_MD_H
#define SCR_BRICK_TETRIS_TETRA_MD_H

#include "field_module_backend.h"
#include "timer.h"

void ShiftTetramino(GameInfo_t *game_info, int direction);

int CheckNewRotate(GameInfo_t *game_info, int new_rotation, int new_x);

void ApplyRotation(GameInfo_t *game_info, int new_rotation, int new_x);

void CheckAndRotateTetramino(GameInfo_t *game_info);

int CheckToShiftTetramino(GameInfo_t *game_info, int direction, int *new_x);

void ShiftTetramino(GameInfo_t *game_info, int new_x);

void ManageTetraminoFromTimer(GameInfo_t *game_info, int test);

void MoveDownTetramino(GameInfo_t *game_info, int is_ghost);

int CanMoveDownTetramino(GameInfo_t *game_info, int is_ghost);

void HardDropTetramino(GameInfo_t *game_info, int is_ghost);

void SetGhostTetramino(GameInfo_t *game_info);

void SpawNewTetramino(GameInfo_t *game_info);

void SetNewTetramino(GameInfo_t *game_info, int new);

#endif