#include "test_config.h"

START_TEST(test_init_field_normal) {
  GameField_t game_field;
  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  for (int i = 0; i < FIELD_SIZE_Y; i++) {
    for (int j = 0; j < FIELD_SIZE_X; j++) {
      ck_assert_int_eq(0, game_field.data[i][j]);
    }
  }
}
END_TEST

START_TEST(test_init_field_null) {
  GameField_t game_field;
  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  InitGameField(NULL, field, FIELD_SIZE_Y);
  InitGameField(&game_field, NULL, FIELD_SIZE_Y);

  ck_assert_ptr_ne(game_field.data, field);
}
END_TEST

START_TEST(test_set_tetramino) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  HardDropTetramino(&game_info, REAL_TETRAMINO);

  for (int i = 0; i < MAX_SHAPE_Y; i++) {
    for (int j = 0; j < MAX_SHAPE_X; j++) {
      if (game_info.current->shape[i][j] != 0 &&
          (i + game_info.current->y) < FIELD_SIZE_Y &&
          (j + game_info.current->x) < FIELD_SIZE_X &&
          (i + game_info.current->y) >= 0 && (j + game_info.current->x) >= 0) {
        ck_assert_uint_eq(
            SHAPES[game_info.current->type][game_info.current->rotation][i][j],
            game_info.field
                ->data[game_info.current->y + i][game_info.current->x + j]);

        /*ck_assert_uint_eq(
            GHOST_TETRAMINO_DEF,
            game_info.field->data[game_info.current->ghost_y + i]
                                 [game_info.current->ghost_x + j]);*/
      }
    }
  }
}

START_TEST(test_delete_tetramino) {
  GameInfo_t game_info;

  GameField_t game_field;

  int field[FIELD_SIZE_Y][FIELD_SIZE_X];

  TetraminesCollection Tetramines;

  InitGameField(&game_field, field, FIELD_SIZE_Y);

  InitAllTetramines(&Tetramines);

  InitGameInfo(&game_info, &game_field, &Tetramines);

  SpawNewTetramino(&game_info);

  DeleteTetraminoFromField(&game_info, REAL_TETRAMINO);

  for (int i = 0; i < MAX_SHAPE_Y; i++) {
    for (int j = 0; j < MAX_SHAPE_X; j++) {
      ck_assert_int_eq(0, game_info.field->data[game_info.current->y + i]
                                               [game_info.current->x + j]);
    }
  }
}
END_TEST

Suite *field_module(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Field Module");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_init_field_normal);
  tcase_add_test(tc_core, test_init_field_null);
  tcase_add_test(tc_core, test_set_tetramino);
  tcase_add_test(tc_core, test_delete_tetramino);
  suite_add_tcase(s, tc_core);

  return s;
}