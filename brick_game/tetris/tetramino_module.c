#include "tetramino_module.h"

void ManageTetraminoFromTimer(GameInfo_t *game_info, int test) {
  if (!game_info->pause && !game_info->game_over && game_info->current &&
      (game_info->current_state == kSTATE_PLAY)) {
    if (ShouldDropPiece(game_info, LEVEL_TIME[game_info->level]) || test) {
      game_info->lastDropTime = getAdjustedTime(game_info);
      if (CanMoveDownTetramino(game_info, REAL_TETRAMINO))
        MoveDownTetramino(game_info, REAL_TETRAMINO);
      if (!CanMoveDownTetramino(game_info, REAL_TETRAMINO)) {
        SetTetraminoToField(game_info, REAL_TETRAMINO);
        game_info->current_state = kSTATE_LINE_CLEAR;
      }
    }
  }
}

void MoveDownTetramino(GameInfo_t *game_info, int is_ghost) {
  DeleteTetraminoFromField(game_info, is_ghost);

  if (is_ghost)
    game_info->current->ghost_y++;
  else {
    game_info->current->y++;
    SetTetraminoToField(game_info, is_ghost);
  }

  game_info->update_request = 1;
}

int CanMoveDownTetramino(GameInfo_t *game_info, int is_ghost) {
  int can_move_down = 1;

  int last_y_in_this_column = -1;

  for (int i = 0; i < MAX_SHAPE_X && can_move_down; ++i) {
    last_y_in_this_column = -1;
    for (int j = 0; j < MAX_SHAPE_Y && can_move_down; j++) {
      int cell = game_info->current->shape[j][i];
      if (cell != 0) {
        last_y_in_this_column = j;
      }
    }
    if (last_y_in_this_column > -1) {
      int shape_x = game_info->current->x + i;
      int shape_y = game_info->current->y + last_y_in_this_column;

      if (is_ghost) {
        shape_x = game_info->current->ghost_x + i;
        shape_y = game_info->current->ghost_y + last_y_in_this_column;
      }

      if (shape_y >= FIELD_SIZE_Y - 1) can_move_down = 0;

      if (can_move_down && (game_info->field->data[shape_y + 1][shape_x] != 0 &&
                            game_info->field->data[shape_y + 1][shape_x] !=
                                GHOST_TETRAMINO_DEF)) {
        can_move_down = 0;
      }
    }
  }

  return can_move_down;
}

int CheckToShiftTetramino(GameInfo_t *game_info, int direction, int *new_x) {
  int can_shift = 1;

  *new_x = game_info->current->x + direction;

  int col_to_check = -1;

  if (*new_x < 0 || *new_x >= FIELD_SIZE_X) can_shift = 0;

  if (direction == -1) {
    for (int j = 0; j < MAX_SHAPE_X && col_to_check < 0; j++) {
      for (int i = 0; i < MAX_SHAPE_Y && col_to_check < 0; i++) {
        if (game_info->current->shape[i][j] != 0) {
          col_to_check = j;
        }
      }
    }
  } else if (direction == 1) {
    for (int j = MAX_SHAPE_X - 1; j >= 0 && col_to_check < 0; j--) {
      for (int i = 0; i < MAX_SHAPE_Y && col_to_check < 0; i++) {
        if (game_info->current->shape[i][j] != 0) {
          col_to_check = j;
        }
      }
    }
  }

  for (int i = 0; i < MAX_SHAPE_Y && can_shift; i++) {
    if (game_info->current->shape[i][col_to_check] != 0 &&
        (col_to_check + *new_x < 0 || col_to_check + *new_x >= FIELD_SIZE_X ||
         game_info->field->data[i + game_info->current->y]
                               [col_to_check + *new_x] != 0)) {
      can_shift = 0;
    }
  }

  return can_shift;
}

void ShiftTetramino(GameInfo_t *game_info, int new_x) {
  if (!(game_info->pause || game_info->game_over)) {
    DeleteTetraminoFromField(game_info, REAL_TETRAMINO);

    DeleteTetraminoFromField(game_info, GHOST_TETRAMINO);

    game_info->current->x = new_x;

    SetGhostTetramino(game_info);

    SetTetraminoToField(game_info, REAL_TETRAMINO);

    game_info->update_request = 1;
  }
}

int CheckNewRotate(GameInfo_t *game_info, int new_rotation, int new_x) {
  int field_x = 0;
  int field_y = 0;

  int can_rotate = 1;

  int last_nonzero_col = 0;

  for (int j = 0; j < MAX_SHAPE_X; j++) {
    for (int i = 0; i < MAX_SHAPE_Y; i++) {
      if (SHAPES[game_info->current->type][new_rotation][i][j] != 0) {
        last_nonzero_col = j;
      }
    }
  }

  DeleteTetraminoFromField(game_info, REAL_TETRAMINO);

  field_x = new_x + last_nonzero_col;

  for (int i = 0; i < MAX_SHAPE_Y && can_rotate; i++) {
    if (SHAPES[game_info->current->type][new_rotation][i][last_nonzero_col] !=
        0) {
      field_y = game_info->current->y + i;

      if (field_x < 0 || field_x >= FIELD_SIZE_X || field_y >= FIELD_SIZE_Y ||
          field_y < 0)
        can_rotate = 0;

      if (can_rotate && game_info->field->data[field_y][field_x] != 0)
        can_rotate = 0;
    }
  }

  if (!can_rotate) SetTetraminoToField(game_info, REAL_TETRAMINO);

  return can_rotate;
}

void ApplyRotation(GameInfo_t *game_info, int new_rotation, int new_x) {
  DeleteTetraminoFromField(game_info, REAL_TETRAMINO);

  DeleteTetraminoFromField(game_info, GHOST_TETRAMINO);

  game_info->current->rotation = new_rotation;

  for (int i = 0; i < MAX_SHAPE_Y; i++) {
    for (int j = 0; j < MAX_SHAPE_X; j++) {
      game_info->current->shape[i][j] =
          SHAPES[game_info->current->type][game_info->current->rotation][i][j];
    }
  }

  game_info->current->x = new_x;

  SetGhostTetramino(game_info);

  SetTetraminoToField(game_info, REAL_TETRAMINO);

  game_info->update_request = 1;
}

void CheckAndRotateTetramino(GameInfo_t *game_info) {
  if (game_info->pause || game_info->game_over) return;

  int new_rotation =
      (game_info->current->rotation + 1) % game_info->current->max_rotations;

  int shift_x = 1;

  if ((game_info->current->type + 1) == kTETRAMINO_I) shift_x = 3;

  if (CheckNewRotate(game_info, new_rotation, game_info->current->x)) {
    ApplyRotation(game_info, new_rotation, game_info->current->x);

  } else if (CheckNewRotate(game_info, new_rotation,
                            game_info->current->x - shift_x)) {
    ApplyRotation(game_info, new_rotation, game_info->current->x - shift_x);
  }
}

void HardDropTetramino(GameInfo_t *game_info, int is_ghost) {
  if (!(game_info->pause || game_info->game_over)) {
    if (!is_ghost) DeleteTetraminoFromField(game_info, GHOST_TETRAMINO);

    int can_move = 1;

    while (can_move) {
      if (CanMoveDownTetramino(game_info, is_ghost))
        MoveDownTetramino(game_info, is_ghost);
      else
        can_move = 0;
    }

    SetTetraminoToField(game_info, is_ghost);

    game_info->update_request = 1;
  }
}

void SetGhostTetramino(GameInfo_t *game_info) {
  game_info->current->ghost_x = game_info->current->x;
  game_info->current->ghost_y = game_info->current->y;

  HardDropTetramino(game_info, GHOST_TETRAMINO);
}

void SetNewTetramino(GameInfo_t *game_info, int new) {
  switch (new + 1) {
    case kTETRAMINO_I:
      game_info->current = &game_info->all->I_Tetramino;
      break;

    case kTETRAMINO_J:
      game_info->current = &game_info->all->J_Tetramino;
      break;

    case kTETRAMINO_L:
      game_info->current = &game_info->all->L_Tetramino;
      break;

    case kTETRAMINO_O:
      game_info->current = &game_info->all->O_Tetramino;
      break;

    case kTETRAMINO_S:
      game_info->current = &game_info->all->S_Tetramino;
      break;

    case kTETRAMINO_T:
      game_info->current = &game_info->all->T_Tetramino;
      break;

    case kTETRAMINO_Z:
      game_info->current = &game_info->all->Z_Tetramino;
      break;
  }
}

static void GenerateNewTetramino(GameInfo_t *game_info) {
  int new = 0;
  int next = 0;

  if (game_info->next_tetr_type < 0) {
    new = rand() % (MAX_SHAPES);
    next = rand() % (MAX_SHAPES);
    game_info->next_tetr_type = next;
  } else {
    new = game_info->next_tetr_type;

    int gen = 1;

    while (gen) {
      next = rand() % (MAX_SHAPES);

      if (next != new &&next != game_info->next_tetr_type) gen = 0;
    }
    game_info->next_tetr_type = next;
  }

  SetNewTetramino(game_info, new);
}

void SpawNewTetramino(GameInfo_t *game_info) {
  if (!(game_info->pause || game_info->game_over)) {
    GenerateNewTetramino(game_info);

    game_info->current->rotation = 0;
    game_info->current->x = TETRAMINO_START_POS_X;
    game_info->current->ghost_x = TETRAMINO_START_POS_X;
    game_info->current->y = 0;
    game_info->current->ghost_y = 0;

    for (int i = 0; i < MAX_SHAPE_Y; i++) {
      for (int j = 0; j < MAX_SHAPE_X; j++) {
        game_info->current->shape[i][j] =
            SHAPES[game_info->current->type][game_info->current->rotation][i]
                  [j];
      }
    }
    SetGhostTetramino(game_info);

    SetTetraminoToField(game_info, REAL_TETRAMINO);

    game_info->update_request = 1;

    game_info->update_new_tetr_request = 1;
  }
}