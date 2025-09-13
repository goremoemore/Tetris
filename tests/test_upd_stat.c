#include "test_config.h"

START_TEST(test_upd_score) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  int scores[4] = {100, 300, 700, 1500};

  for (int i = 0; i < 4; i++) {
    game_info.score = 0;
    UpdateScore(&game_info, i + 1);

    ck_assert_uint_eq(scores[i], game_info.score);
  }
}
END_TEST

START_TEST(test_upd_high_score) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  game_info.score = 0;
  game_info.high_score = 300;
  UpdateScore(&game_info, 1);

  ck_assert_uint_eq(game_info.high_score, 300);

  UpdateScore(&game_info, 4);

  ck_assert_uint_eq(game_info.high_score, 1600);
}
END_TEST

START_TEST(test_upd_level) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);
  game_info.score = 600;

  UpdateLevel(&game_info);
  ck_assert_uint_eq(1, game_info.level);

  game_info.score = 1200;

  UpdateLevel(&game_info);
  ck_assert_uint_eq(2, game_info.level);

  game_info.level = MAX_LEVELS - 1;
  UpdateLevel(&game_info);
  ck_assert_uint_eq(MAX_LEVELS - 1, game_info.level);
}
END_TEST

Suite *upd(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Update Stat");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_upd_score);
  tcase_add_test(tc_core, test_upd_high_score);
  tcase_add_test(tc_core, test_upd_level);
  suite_add_tcase(s, tc_core);

  return s;
}