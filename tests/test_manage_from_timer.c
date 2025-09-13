#include "test_config.h"

START_TEST(test_can_move_after_drop) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  game_info.current_state = kSTATE_PLAY;

  ManageTetraminoFromTimer(&game_info, 1);

  ck_assert_int_eq(CanMoveDownTetramino(&game_info, REAL_TETRAMINO), 1);
}
END_TEST

START_TEST(test_cant_move_after_drop) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  HardDropTetramino(&game_info, REAL_TETRAMINO);

  game_info.current_state = kSTATE_PLAY;
  ManageTetraminoFromTimer(&game_info, 1);

  ck_assert_int_eq(CanMoveDownTetramino(&game_info, REAL_TETRAMINO), 0);
}
END_TEST

Suite *timer_move(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("MoveFromTimer");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_can_move_after_drop);
  tcase_add_test(tc_core, test_cant_move_after_drop);
  suite_add_tcase(s, tc_core);

  return s;
}