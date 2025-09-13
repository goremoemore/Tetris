#include "test_config.h"

START_TEST(test_rotate_if_pause_or_over) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  int last_rotation = game_info.current->rotation;

  game_info.pause = 1;
  CheckAndRotateTetramino(&game_info);

  ck_assert_int_eq(last_rotation, game_info.current->rotation);

  game_info.game_over = 1;
  CheckAndRotateTetramino(&game_info);

  ck_assert_int_eq(last_rotation, game_info.current->rotation);
}
END_TEST

START_TEST(test_rotate_normal) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  int last_rotation = game_info.current->rotation;

  CheckAndRotateTetramino(&game_info);

  ck_assert_int_ne(last_rotation, game_info.current->rotation);
}
END_TEST

START_TEST(test_rotate_normal_with_kick) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  DeleteTetraminoFromField(&game_info, REAL_TETRAMINO);

  game_info.current = &game_info.all->I_Tetramino;
  game_info.current->rotation = 0;
  game_info.current->x = TETRAMINO_START_POS_X;
  game_info.current->ghost_x = TETRAMINO_START_POS_X;
  game_info.current->y = 0;
  game_info.current->ghost_y = 0;

  for (int i = 0; i < MAX_SHAPE_Y; i++) {
    for (int j = 0; j < MAX_SHAPE_X; j++) {
      game_info.current->shape[i][j] =
          SHAPES[0][game_info.current->rotation][i][j];
    }
  }

  SetTetraminoToField(&game_info, REAL_TETRAMINO);

  int new_x = -1;
  while (CheckToShiftTetramino(&game_info, 1, &new_x)) {
    ShiftTetramino(&game_info, new_x);
  }

  int last_rotation = game_info.current->rotation;

  CheckAndRotateTetramino(&game_info);

  ck_assert_int_ne(last_rotation, game_info.current->rotation);
}
END_TEST

Suite *rotate(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Rotate");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_rotate_if_pause_or_over);
  tcase_add_test(tc_core, test_rotate_normal);
  tcase_add_test(tc_core, test_rotate_normal_with_kick);
  suite_add_tcase(s, tc_core);

  return s;
}