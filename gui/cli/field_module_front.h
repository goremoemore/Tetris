#ifndef SRC_GUI_CLI_FIELD_FRONT_H
#define SRC_GUI_CLI_FIELD_FRONT_H

#include <locale.h>
#include <ncursesw/ncurses.h>

#include "tetris_api.h"

#define DRAW_START_Y 2
#define DRAW_START_X 2

#define NEXT_TET_DRAW_START_Y 11
#define NEXT_TET_DRAW_START_X 45
#define TEXT_INFO_X 5
#define TEXT_INFO_Y 0

#define BRIGHTNESS 350
#define DIM_COLORS_NUM_SHIFT 20

typedef enum {
  kRED_I_TETR = 1,
  kWHITE_J_TETR,
  kYELLOW_L_TETR,
  kCYAN_O_TETR,
  kGREEN_S_TETR,
  kBLUE_T_TETR,
  kMAGNETA_Z_TETR,
  kEMPTY_CELL,
  kBORDERS,
} Colors;

typedef enum {
  kDIM_RED_I_TETR = kRED_I_TETR + DIM_COLORS_NUM_SHIFT,
  kDIM_WHITE_J_TETR,
  kDIM_YELLOW_L_TETR,
  kDIM_CYAN_O_TETR,
  kDIM_GREEN_S_TETR,
  kDIM_BLUE_T_TETR,
  kDIM_MAGNETA_Z_TETR,
} DimColors;

void InitCurses(int part);

void InitGameFront();

void InitColors();

void DrawGameField();

void UpdateNextTeramino(GameInfo_t *game_info);

void UpdateCurrentState(GameInfo_t *game_info);

void UpdateInformation(GameInfo_t *game_info);

void AnimateClearLines(int start_line, int line_count);

void ShowSomeInfo(int y, int x, char *text);

void ClearSomeInfo(int y, int x, char *text);

void ShowTopPlayers(TableHighScoresTop10 data, const char *name);

#endif
