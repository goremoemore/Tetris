#include "test_config.h"

START_TEST(test_shift_left) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  int new_x = -1;

  int last_x = game_info.current->x;

  ck_assert_int_eq(1, CheckToShiftTetramino(&game_info, -1, &new_x));
  ShiftTetramino(&game_info, new_x);
  ck_assert_int_eq(last_x - 1, game_info.current->x);
}
END_TEST

START_TEST(test_shift_right) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  int new_x = -1;

  int last_x = game_info.current->x;

  if (CheckToShiftTetramino(&game_info, 1, &new_x))
    ShiftTetramino(&game_info, new_x);

  ck_assert_int_eq(last_x + 1, game_info.current->x);
}
END_TEST

START_TEST(test_shift_cant_shift_left) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  int new_x = -1;
  while (CheckToShiftTetramino(&game_info, -1, &new_x)) {
    ShiftTetramino(&game_info, new_x);
  }

  ck_assert_int_eq(CheckToShiftTetramino(&game_info, -1, &new_x), 0);
}
END_TEST

START_TEST(test_shift_cant_shift_right) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  int new_x = -1;
  while (CheckToShiftTetramino(&game_info, 1, &new_x)) {
    ShiftTetramino(&game_info, new_x);
  }

  ck_assert_int_eq(CheckToShiftTetramino(&game_info, 1, &new_x), 0);
}
END_TEST

Suite *shift(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Shift");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_shift_left);
  tcase_add_test(tc_core, test_shift_right);
  tcase_add_test(tc_core, test_shift_cant_shift_left);
  tcase_add_test(tc_core, test_shift_cant_shift_right);
  suite_add_tcase(s, tc_core);

  return s;
}