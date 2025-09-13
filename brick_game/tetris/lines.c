#include "lines.h"

void CheckLinesForClear(GameInfo_t *game_info) {
  int first_line_cleared = -1;
  int count_line_cleared = 0;
  int checker = 0;

  for (int i = (FIELD_SIZE_Y - 1); i >= 0; i--) {
    checker = 1;

    for (int j = 0; j < FIELD_SIZE_X; j++) {
      checker *= game_info->field->data[i][j];
      if (!checker) break;
    }

    if (checker > 0) {
      if (first_line_cleared == -1) first_line_cleared = i;
      count_line_cleared += 1;
    }
  }

  game_info->start_line_for_animated_clear = first_line_cleared;
  game_info->count_lines_for_animated_clear = count_line_cleared;
  game_info->animate_clear_lines_request = count_line_cleared > 0;
}

void ClearLines(GameInfo_t *game_info, int start_line, int line_count) {
  for (int i = start_line; i > start_line - line_count; i--) {
    for (int j = 0; j < FIELD_SIZE_X; j++) game_info->field->data[i][j] = 0;
  }
}

void ShiftLines(GameInfo_t *game_info, int start_line, int line_count) {
  int first_nonzero_row = -1;

  for (int i = 0; i < FIELD_SIZE_Y && first_nonzero_row < 0; i++) {
    for (int j = 0; j < FIELD_SIZE_X && first_nonzero_row < 0; j++) {
      if (game_info->field->data[i][j] != 0) {
        first_nonzero_row = i;
      }
    }
  }

  for (int i = 0; (i <= start_line - line_count - first_nonzero_row) &&
                  first_nonzero_row > -1;
       i++) {
    for (int j = 0; j < FIELD_SIZE_X; j++) {
      game_info->field->data[start_line - i][j] =
          game_info->field->data[start_line - line_count - i][j];
      game_info->field->data[start_line - line_count - i][j] = 0;
    }
  }

  game_info->update_request = 1;
}