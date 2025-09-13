#include "test_config.h"

START_TEST(test_hard_drop_real) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  int last_y = 0;

  HardDropTetramino(&game_info, REAL_TETRAMINO);

  CheckTetraminoOnField(&game_info);

  ck_assert_int_ne(last_y, game_info.current->y);
}
END_TEST

START_TEST(test_hard_drop_ghost) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  int last_y = 0;

  for (int i = 0; i < MAX_SHAPE_Y; i++) {
    for (int j = 0; j < MAX_SHAPE_X; j++) {
      if (game_info.current->shape[i][j] != 0 &&
          (i + game_info.current->ghost_y) < FIELD_SIZE_Y &&
          (j + game_info.current->ghost_x) < FIELD_SIZE_X &&
          (i + game_info.current->ghost_y) >= 0 &&
          (j + game_info.current->ghost_x) >= 0) {
        ck_assert_uint_eq(
            GHOST_TETRAMINO_DEF,
            game_info.field->data[game_info.current->ghost_y + i]
                                 [game_info.current->ghost_x + j]);
      }
    }
  }

  ck_assert_int_ne(last_y, game_info.current->ghost_y);
}
END_TEST

START_TEST(test_hard_drop_if_pause_or_over) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  game_info.pause = 1;

  int last_y = game_info.current->y;

  HardDropTetramino(&game_info, REAL_TETRAMINO);

  ck_assert_int_eq(last_y, game_info.current->y);

  game_info.pause = 0;
  game_info.game_over = 1;

  HardDropTetramino(&game_info, REAL_TETRAMINO);

  ck_assert_int_eq(last_y, game_info.current->y);
}
END_TEST

Suite *hard_drop(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Hard Drop");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_hard_drop_real);
  tcase_add_test(tc_core, test_hard_drop_ghost);
  tcase_add_test(tc_core, test_hard_drop_if_pause_or_over);
  suite_add_tcase(s, tc_core);

  return s;
}