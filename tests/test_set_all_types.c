#include "test_config.h"

START_TEST(test_set_all_types) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  for (int i = 0; i < MAX_SHAPES; i++) {
    game_info.current = NULL;
    SetNewTetramino(&game_info, i);
    ck_assert_ptr_ne(&game_info.current, NULL);
  }
}
END_TEST

START_TEST(test_gen_extra_tetramino) {
  GameInfo_t *game_info = InitGame();

  SpawNewTetramino(game_info);

  HardDropTetramino(game_info, REAL_TETRAMINO);

  SpawNewTetramino(game_info);

  ck_assert_int_ne(game_info->next_tetr_type, -1);

  EndGame(game_info);
}
END_TEST

Suite *set_types(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("SpawnTetramines");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_set_all_types);
  tcase_add_test(tc_core, test_gen_extra_tetramino);
  suite_add_tcase(s, tc_core);

  return s;
}