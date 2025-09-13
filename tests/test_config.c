#include "test_config.h"

void CheckTetraminoOnField(GameInfo_t *game_info) {
  for (int i = 0; i < MAX_SHAPE_Y; i++) {
    for (int j = 0; j < MAX_SHAPE_X; j++) {
      if (game_info->current->shape[i][j] != 0 &&
          (i + game_info->current->y) < FIELD_SIZE_Y &&
          (j + game_info->current->x) < FIELD_SIZE_X &&
          (i + game_info->current->y) >= 0 &&
          (j + game_info->current->x) >= 0) {
        ck_assert_uint_eq(SHAPES[game_info->current->type]
                                [game_info->current->rotation][i][j],
                          game_info->field->data[game_info->current->y + i]
                                                [game_info->current->x + j]);
      }
    }
  }
}