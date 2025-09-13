#include "test_config.h"

START_TEST(test_check_lines_for_clear) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);
  for (int i = 0; i < 9; i++) {
    ck_assert_int_eq(0, game_info.field->data[FIELD_SIZE_Y - 1][i]);
  }
  for (int i = 0; i < FIELD_SIZE_X; i++) {
    game_info.field->data[FIELD_SIZE_Y - 1][i] = 1;
  }

  CheckLinesForClear(&game_info);
  ck_assert_int_eq(game_info.count_lines_for_animated_clear, 1);
  ck_assert_int_eq(game_info.start_line_for_animated_clear, 19);
}
END_TEST

START_TEST(test_clear_lines) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  for (int i = 0; i < FIELD_SIZE_X; i++) {
    game_info.field->data[FIELD_SIZE_Y - 1][i] = 1;
  }

  ClearLines(&game_info, FIELD_SIZE_Y - 1, 1);

  for (int i = 0; i < FIELD_SIZE_X; i++) {
    ck_assert_int_eq(game_info.field->data[FIELD_SIZE_Y - 1][i], 0);
  }
}
END_TEST

START_TEST(test_shift_lines) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  for (int i = 0; i < FIELD_SIZE_X; i++) {
    game_info.field->data[FIELD_SIZE_Y - 1][i] = 1;
    game_info.field->data[FIELD_SIZE_Y - 2][i] = 2;
  }

  ClearLines(&game_info, FIELD_SIZE_Y - 1, 1);
  ShiftLines(&game_info, FIELD_SIZE_Y - 1, 1);

  for (int i = 0; i < FIELD_SIZE_X; i++) {
    ck_assert_int_eq(game_info.field->data[FIELD_SIZE_Y - 1][i], 2);
  }
}
END_TEST

Suite *lines(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Clear Lines");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_check_lines_for_clear);
  tcase_add_test(tc_core, test_clear_lines);
  tcase_add_test(tc_core, test_shift_lines);
  suite_add_tcase(s, tc_core);

  return s;
}