#include "field_module_backend.h"

void SetTetraminoToField(GameInfo_t *game_info, int is_ghost) {
  int shape_x = game_info->current->x;
  int shape_y = game_info->current->y;
  int filler = 0;

  if (is_ghost) {
    shape_x = game_info->current->ghost_x;
    shape_y = game_info->current->ghost_y;
    filler = GHOST_TETRAMINO_DEF;
  }

  for (int i = 0; i < MAX_SHAPE_Y; ++i) {
    for (int j = 0; j < MAX_SHAPE_X; ++j) {
      if (!is_ghost) filler = game_info->current->shape[i][j];
      if (game_info->current->shape[i][j] != 0 &&
          (i + shape_y) < FIELD_SIZE_Y && (j + shape_x) < FIELD_SIZE_X &&
          (i + shape_y) >= 0 && (j + shape_x) >= 0) {
        game_info->field->data[i + shape_y][j + shape_x] = filler;
      }
    }
  }
}

void DeleteTetraminoFromField(GameInfo_t *game_info, int is_ghost) {
  int shape_x = game_info->current->x;
  int shape_y = game_info->current->y;

  if (is_ghost) {
    shape_x = game_info->current->ghost_x;
    shape_y = game_info->current->ghost_y;
  }

  for (int i = 0; i < MAX_SHAPE_Y; i++) {
    for (int j = 0; j < MAX_SHAPE_X; j++) {
      if (game_info->current->shape[i][j] != 0 &&
          (i + shape_y) < FIELD_SIZE_Y && (j + shape_x) < FIELD_SIZE_X &&
          (i + shape_y) >= 0 && (j + shape_x >= 0))
        game_info->field->data[shape_y + i][shape_x + j] = 0;
    }
  }
}