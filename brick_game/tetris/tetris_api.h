#ifndef SCR_BRICK_TETIRIS_API_H
#define SCR_BRICK_TETIRIS_API_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

#include "database.h"

#define FIELD_SIZE_Y 20
#define FIELD_SIZE_X 10
#define MAX_SHAPE_Y 4
#define MAX_SHAPE_X 4
#define MAX_SHAPES 7
#define MAX_SHAPE_ROTATIONS 4

#define MAX_LEVELS 10
#define UPD_LEVEL_SCORE 600

#define REAL_TETRAMINO 0
#define GHOST_TETRAMINO 1
#define GHOST_TETRAMINO_DEF (MAX_SHAPES + 1)

#define kTETRAMINO_I 1
#define kTETRAMINO_J 2
#define kTETRAMINO_L 3
#define kTETRAMINO_O 4
#define kTETRAMINO_S 5
#define kTETRAMINO_T 6
#define kTETRAMINO_Z 7

#define TETRAMINO_START_POS_X 3

typedef struct {
  int (*data)[FIELD_SIZE_X];
  int rows;
} GameField_t;

typedef struct {
  int type;
  int rotation;
  int max_rotations;
  int shape[MAX_SHAPE_Y][MAX_SHAPE_X];
  int y;
  int x;
  int ghost_y;
  int ghost_x;
} Tetramino;

typedef struct {
  Tetramino I_Tetramino;
  Tetramino J_Tetramino;
  Tetramino L_Tetramino;
  Tetramino O_Tetramino;
  Tetramino S_Tetramino;
  Tetramino T_Tetramino;
  Tetramino Z_Tetramino;
} TetraminesCollection;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  ShowScores
} UserAction_t;

typedef struct {
  GameField_t *field;
  int field_data[FIELD_SIZE_Y][FIELD_SIZE_X];
  Tetramino *current;
  TetraminesCollection *all;
  int next_tetr_type;
  char player[10];
  int score;
  int last_saved_score;
  int total_line_cleared;
  int high_score;
  int level;
  int speed;
  int pause;
  int game_over;
  int current_state;
  int update_request;
  int update_new_tetr_request;
  int update_some_info;
  int animate_clear_lines_request;
  int animated_clear_lines_done;
  int start_line_for_animated_clear;
  int count_lines_for_animated_clear;
  int show_top_players_request;
  TableHighScoresTop10 top_players;
  clock_t lastDropTime;
  clock_t pauseStartTime;
  clock_t accumulatedPauseTime;
} GameInfo_t;

typedef enum {
  kSTATE_NEW_PLAYER,
  kSTATE_MAIN_MENU,
  kSTATE_SHOW_SCORES,
  kSTATE_FIRST_START,
  kSTATE_PLAY,
  kSTATE_PAUSE,
  kSTATE_LINE_CLEAR,
  kSTATE_SPAWN_NEW,
  kSTATE_GAME_OVER,
  kSTATE_TERMINATE,
  kSTATE_WAITING_FOR_EXIT
} GameState_t;

static const int LEVEL_TIME[MAX_LEVELS] = {1100, 1000, 900, 800, 600,
                                           500,  350,  250, 200, 100};
static const int
    SHAPES[MAX_SHAPES][MAX_SHAPE_ROTATIONS][MAX_SHAPE_Y][MAX_SHAPE_X] = {
        {{{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}},
         {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}}},
        {{{2, 0, 0, 0}, {2, 2, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{2, 2, 0, 0}, {2, 0, 0, 0}, {2, 0, 0, 0}, {0, 0, 0, 0}},
         {{2, 2, 2, 0}, {0, 0, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{0, 2, 0, 0}, {0, 2, 0, 0}, {2, 2, 0, 0}, {0, 0, 0, 0}}},
        {{{0, 0, 3, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{3, 0, 0, 0}, {3, 0, 0, 0}, {3, 3, 0, 0}, {0, 0, 0, 0}},
         {{0, 0, 0, 0}, {3, 3, 3, 0}, {3, 0, 0, 0}, {0, 0, 0, 0}},
         {{3, 3, 0, 0}, {0, 3, 0, 0}, {0, 3, 0, 0}, {0, 0, 0, 0}}},
        {{{4, 4, 0, 0}, {4, 4, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{4, 4, 0, 0}, {4, 4, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{4, 4, 0, 0}, {4, 4, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{4, 4, 0, 0}, {4, 4, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
        {{{0, 5, 5, 0}, {5, 5, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{5, 0, 0, 0}, {5, 5, 0, 0}, {0, 5, 0, 0}, {0, 0, 0, 0}},
         {{0, 5, 5, 0}, {5, 5, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{5, 0, 0, 0}, {5, 5, 0, 0}, {0, 5, 0, 0}, {0, 0, 0, 0}}},
        {{{0, 6, 0, 0}, {6, 6, 6, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{6, 0, 0, 0}, {6, 6, 0, 0}, {6, 0, 0, 0}, {0, 0, 0, 0}},
         {{6, 6, 6, 0}, {0, 6, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{0, 6, 0, 0}, {6, 6, 0, 0}, {0, 6, 0, 0}, {0, 0, 0, 0}}},
        {{{7, 7, 0, 0}, {0, 7, 7, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{0, 7, 0, 0}, {7, 7, 0, 0}, {7, 0, 0, 0}, {0, 0, 0, 0}},
         {{7, 7, 0, 0}, {0, 7, 7, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{0, 7, 0, 0}, {7, 7, 0, 0}, {7, 0, 0, 0}, {0, 0, 0, 0}}}};

void InitGameField(GameField_t *game_field, int (*field)[FIELD_SIZE_X],
                   int rows);

Tetramino CreateTetramino(int type);

void InitAllTetramines(TetraminesCollection *t);

void InitGameInfo(GameInfo_t *game_info, GameField_t *game_field,
                  TetraminesCollection *all);

GameInfo_t *InitGame();

void EndGame(GameInfo_t *game_info);

#endif