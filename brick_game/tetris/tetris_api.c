#include "tetris_api.h"

GameInfo_t *InitGame() {
  GameInfo_t *game_info = malloc(sizeof(GameInfo_t));

  GameField_t *game_field = malloc(sizeof(GameField_t));

  TetraminesCollection *Tetramines = malloc(sizeof(TetraminesCollection));

  InitDataBase();

  InitGameField(game_field, game_info->field_data, FIELD_SIZE_Y);

  InitAllTetramines(Tetramines);

  InitGameInfo(game_info, game_field, Tetramines);

  return game_info;
}

void EndGame(GameInfo_t *game_info) {
  CloseDataBase();
  free(game_info->field);
  free(game_info->all);
  free(game_info);
}

void InitGameField(GameField_t *game_field, int (*field)[FIELD_SIZE_X],
                   int rows) {
  if (!game_field || !field) return;

  game_field->data = field;
  game_field->rows = rows;

  for (int i = 0; i < FIELD_SIZE_Y; i++) {
    for (int j = 0; j < FIELD_SIZE_X; j++) {
      game_field->data[i][j] = 0;
    }
  }
}

Tetramino CreateTetramino(int type) {
  Tetramino t;
  t.type = type - 1;
  t.rotation = 0;
  t.x = TETRAMINO_START_POS_X;
  t.y = 0;
  t.ghost_x = TETRAMINO_START_POS_X;
  t.ghost_y = 0;

  switch (type) {
    case kTETRAMINO_I:
      t.max_rotations = 2;
      break;
    case kTETRAMINO_J:
      t.max_rotations = 4;
      break;
    case kTETRAMINO_L:
      t.max_rotations = 4;
      break;
    case kTETRAMINO_O:
      t.max_rotations = 2;
      break;
    case kTETRAMINO_S:
      t.max_rotations = 2;
      break;
    case kTETRAMINO_T:
      t.max_rotations = 4;
      break;
    case kTETRAMINO_Z:
      t.max_rotations = 2;
      break;
  }

  for (int i = 0; i < MAX_SHAPE_Y; i++) {
    for (int j = 0; j < MAX_SHAPE_X; j++) {
      t.shape[i][j] = SHAPES[t.type][0][i][j];
    }
  }

  return t;
}

void InitAllTetramines(TetraminesCollection *t) {
  t->I_Tetramino = CreateTetramino(kTETRAMINO_I);
  t->J_Tetramino = CreateTetramino(kTETRAMINO_J);
  t->L_Tetramino = CreateTetramino(kTETRAMINO_L);
  t->O_Tetramino = CreateTetramino(kTETRAMINO_O);
  t->S_Tetramino = CreateTetramino(kTETRAMINO_S);
  t->T_Tetramino = CreateTetramino(kTETRAMINO_T);
  t->Z_Tetramino = CreateTetramino(kTETRAMINO_Z);
}

void InitGameInfo(GameInfo_t *game_info, GameField_t *field,
                  TetraminesCollection *all) {
  srand(time(NULL));

  game_info->field = field;
  game_info->current_state = kSTATE_NEW_PLAYER;
  game_info->all = all;
  game_info->lastDropTime = clock();
  game_info->accumulatedPauseTime = 0;
  game_info->pauseStartTime = 0;
  game_info->score = 0;
  game_info->high_score = 0;
  game_info->total_line_cleared = 0;
  game_info->last_saved_score = 0;
  game_info->level = 0;
  game_info->speed = 0;
  game_info->pause = 0;
  game_info->game_over = 0;
  game_info->current = NULL;
  game_info->next_tetr_type = -1;
  game_info->update_request = 0;
  game_info->update_new_tetr_request = 0;
  game_info->update_some_info = 0;
  game_info->animate_clear_lines_request = 0;
  game_info->animated_clear_lines_done = 0;
  game_info->start_line_for_animated_clear = 0;
  game_info->count_lines_for_animated_clear = 0;
  game_info->show_top_players_request = 0;
}