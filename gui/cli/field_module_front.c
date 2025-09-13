#include "field_module_front.h"

void InitCurses(int part) {
  if (!part) {
    setlocale(LC_ALL, "C.UTF-8");
    initscr();
    cbreak();
    echo();
  } else {
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    clear();
  }
}

void InitGameFront() {
  InitColors();
  DrawGameField();
}

void InitColors() {
  start_color();
  use_default_colors();

  init_color(kDIM_RED_I_TETR, BRIGHTNESS, 0, 0);
  init_color(kDIM_WHITE_J_TETR, BRIGHTNESS, BRIGHTNESS, BRIGHTNESS);
  init_color(kDIM_YELLOW_L_TETR, BRIGHTNESS, BRIGHTNESS, 0);
  init_color(kDIM_CYAN_O_TETR, 0, BRIGHTNESS, BRIGHTNESS);
  init_color(kDIM_GREEN_S_TETR, 0, BRIGHTNESS, 0);
  init_color(kDIM_BLUE_T_TETR, 0, 0, BRIGHTNESS);
  init_color(kDIM_MAGNETA_Z_TETR, BRIGHTNESS, 0, BRIGHTNESS);

  init_pair(kRED_I_TETR, COLOR_WHITE, COLOR_RED);
  init_pair(kWHITE_J_TETR, COLOR_WHITE, COLOR_WHITE);
  init_pair(kYELLOW_L_TETR, COLOR_WHITE, COLOR_YELLOW);
  init_pair(kCYAN_O_TETR, COLOR_WHITE, COLOR_CYAN);
  init_pair(kGREEN_S_TETR, COLOR_WHITE, COLOR_GREEN);
  init_pair(kBLUE_T_TETR, COLOR_WHITE, COLOR_BLUE);
  init_pair(kMAGNETA_Z_TETR, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(kEMPTY_CELL, COLOR_WHITE, -1);
  init_pair(kBORDERS, COLOR_WHITE, -1);

  for (int i = 0; i < MAX_SHAPES; i++) {
    init_pair(i + 1 + DIM_COLORS_NUM_SHIFT, COLOR_WHITE,
              i + 1 + DIM_COLORS_NUM_SHIFT);
  }
}

void DrawGameField() {
  int rows = FIELD_SIZE_Y;
  int cols = FIELD_SIZE_X;

  clear();

  attron(COLOR_PAIR(kBORDERS));

  mvprintw(DRAW_START_Y, DRAW_START_X, "┌");
  for (int j = 0; j < cols; j++) {
    printw("───");
    if (j < cols - 1) printw("┬");
  }
  printw("┐");

  for (int i = 0; i < rows; i++) {
    if (i > 0) {
      mvprintw(DRAW_START_Y + i * 2, DRAW_START_X, "├");
      for (int j = 0; j < cols; j++) {
        printw("───");
        if (j < cols - 1) printw("┼");
      }
      printw("┤");
    }

    mvprintw(DRAW_START_Y + i * 2 + 1, DRAW_START_X, "│");
    for (int j = 0; j < cols; j++) {
      attron(COLOR_PAIR(kEMPTY_CELL));

      printw("   ");
      attron(COLOR_PAIR(kBORDERS));
      printw("│");
    }
  }

  mvprintw(DRAW_START_Y + rows * 2, DRAW_START_X, "└");
  for (int j = 0; j < cols; j++) {
    printw("───");
    if (j < cols - 1) printw("┴");
  }
  printw("┘");

  attroff(COLOR_PAIR(kBORDERS));

  refresh();
}

void UpdateCurrentState(GameInfo_t *game_info) {
  for (int i = 0; i < FIELD_SIZE_Y; i++) {
    for (int j = 0; j < FIELD_SIZE_X; j++) {
      int y_pos = DRAW_START_Y + i * 2 + 1;
      int x_pos = DRAW_START_X + 1 + j * 4;

      move(y_pos, x_pos);

      if (game_info->field->data[i][j] == 0) {
        attron(COLOR_PAIR(kEMPTY_CELL));
      } else {
        if (game_info->field->data[i][j] == GHOST_TETRAMINO_DEF)
          attron(
              COLOR_PAIR(game_info->current->type + 1 + DIM_COLORS_NUM_SHIFT));
        else
          attron(COLOR_PAIR(game_info->field->data[i][j]));
      }

      printw("   ");

      attroff(COLOR_PAIR(kEMPTY_CELL));
      attroff(COLOR_PAIR(game_info->field->data[i][j]));
      attroff(COLOR_PAIR(game_info->field->data[i][j] + DIM_COLORS_NUM_SHIFT));
    }
  }

  UpdateInformation(game_info);

  refresh();
}

void UpdateNextTeramino(GameInfo_t *game_info) {
  for (int i = 0; i < MAX_SHAPE_Y; i++) {
    for (int j = 0; j < MAX_SHAPE_X; j++) {
      int y_pos = NEXT_TET_DRAW_START_Y + i * 2 + 1;
      int x_pos = NEXT_TET_DRAW_START_X + 1 + j * 4;

      move(y_pos, x_pos);
      if (SHAPES[game_info->next_tetr_type][0][i][j] == 0) {
        attron(COLOR_PAIR(kEMPTY_CELL));
      } else {
        attron(COLOR_PAIR(SHAPES[game_info->next_tetr_type][0][i][j]));
      }

      printw("   ");

      attroff(COLOR_PAIR(kEMPTY_CELL));
      attroff(COLOR_PAIR(SHAPES[game_info->next_tetr_type][0][i][j]));
    }
  }

  UpdateInformation(game_info);

  refresh();
}

void UpdateInformation(GameInfo_t *game_info) {
  mvprintw(3, 45, "PLAYER:");
  mvprintw(4, 45, "%s", game_info->player);

  mvprintw(6, 45, "HIGH SCORE:");
  mvprintw(7, 45, "%d", game_info->high_score);

  mvprintw(10, NEXT_TET_DRAW_START_X, "NEXT");

  mvprintw(17, NEXT_TET_DRAW_START_X, "LEVEL:");
  mvprintw(18, NEXT_TET_DRAW_START_X, "%d/%d", game_info->level + 1,
           MAX_LEVELS);

  mvprintw(20, NEXT_TET_DRAW_START_X, "SCORE:");
  mvprintw(21, NEXT_TET_DRAW_START_X, "%d", game_info->score);

  mvprintw(23, NEXT_TET_DRAW_START_X, "LINES:");
  mvprintw(24, NEXT_TET_DRAW_START_X, "%d", game_info->total_line_cleared);
}

void ShowSomeInfo(int y, int x, char *text) {
  mvprintw(y, x, "%s", text);
  refresh();
}

void ClearSomeInfo(int y, int x, char *text) {
  for (size_t i = 0; i < strlen(text); i++) mvprintw(y, x + i, " ");
  refresh();
}

static void msleep(long milliseconds) {
  struct timespec ts = {.tv_sec = milliseconds / 1000,
                        .tv_nsec = (milliseconds % 1000) * 1000000};

  thrd_sleep(&ts, NULL);
}

void AnimateClearLines(int start_line, int line_count) {
  for (int i = start_line; i > start_line - line_count; i--) {
    for (int j = 0; j < FIELD_SIZE_X; j++) {
      move(DRAW_START_Y + i * 2 + 1, DRAW_START_X + j * 4 + 1);
      attron(COLOR_PAIR(kEMPTY_CELL));
      printw("   ");
      refresh();
      msleep(30);
    }
  }
}

void ShowTopPlayers(TableHighScoresTop10 data, const char *name) {
  clear();

  int step_y_text = 2;
  for (int i = 0; i < data.rows_exec; i++) {
    if (!strcmp(data.player_name[i], name)) attron(COLOR_PAIR(kDIM_RED_I_TETR));

    mvprintw(7 + i * step_y_text, 5, "%d   %s   %d", i + 1, data.player_name[i],
             data.score[i]);

    attroff(COLOR_PAIR(kDIM_RED_I_TETR));
  }

  ShowSomeInfo(TEXT_INFO_Y, TEXT_INFO_X, "TO EXIT PRESS \"ESC\"");

  ShowSomeInfo(TEXT_INFO_Y + 4, TEXT_INFO_X + 5, "TOP 10 BEST PLAYERS");
}