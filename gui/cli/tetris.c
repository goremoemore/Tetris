#include "tetris.h"

static int ManageEnteredKey(int key) {
  int result = -1;
  switch (key) {
    case KEY_UP:
      break;

    case KEY_DOWN:
      result = Down;
      break;

    case KEY_LEFT:
      result = Left;
      break;

    case KEY_RIGHT:
      result = Right;
      break;

    case 'p':
      result = Pause;
      break;

    case 32:
      result = Action;
      break;

    case 27:
      result = Terminate;
      break;

    case 's':
      result = ShowScores;
      break;

    case 10:
      result = Start;
      break;

    default:
      break;
  }

  return result;
}

static void UpdateTextInfo(int state) {
  switch (state) {
    case kSTATE_MAIN_MENU:
      ShowSomeInfo(TEXT_INFO_Y, TEXT_INFO_X + 7, "PRESS \"ENTER\" TO START");
      break;

    case kSTATE_FIRST_START:
      ClearSomeInfo(TEXT_INFO_Y, TEXT_INFO_X + 7, "PRESS \"ENTER\" TO START");
      break;

    case kSTATE_GAME_OVER:
      ShowSomeInfo(TEXT_INFO_Y, TEXT_INFO_X,
                   "GAME IS OVER. TO SEE THE SCORE TABLE PRESS \"S\"");
      ShowSomeInfo(TEXT_INFO_Y + 1, TEXT_INFO_X + 14, "TO EXIT PRESS \"ESC\"");
      break;

    case kSTATE_PAUSE:
      ShowSomeInfo(TEXT_INFO_Y, TEXT_INFO_X,
                   "GAME IS PAUSED. PRESS \"P\" TO UNPAUSE");
      break;

    case kSTATE_PLAY:
      ClearSomeInfo(TEXT_INFO_Y, TEXT_INFO_X,
                    "GAME IS PAUSED. PRESS \"P\" TO UNPAUSE");
      break;
  }
}

static void ManageInfoByState(GameInfo_t *game_info) {
  if (game_info->update_request) {
    UpdateCurrentState(game_info);
    game_info->update_request = 0;
  }

  if (game_info->update_new_tetr_request) {
    UpdateNextTeramino(game_info);
    game_info->update_new_tetr_request = 0;
  }

  if (game_info->animate_clear_lines_request) {
    AnimateClearLines(game_info->start_line_for_animated_clear,
                      game_info->count_lines_for_animated_clear);
    game_info->animated_clear_lines_done = 1;
  }

  if (game_info->show_top_players_request) {
    ShowTopPlayers(game_info->top_players, game_info->player);
    game_info->show_top_players_request = 0;
  }

  if (game_info->update_some_info) {
    UpdateTextInfo(game_info->current_state);
    game_info->update_some_info = 0;
  }
}

void Tetris() {
  InitCurses(0);

  GameInfo_t *game_info = InitGame();

  EnterPlayerName(game_info);

  InitCurses(1);

  InitGameFront();

  while (game_info->current_state != kSTATE_TERMINATE) {
    GameProcess(game_info, ManageEnteredKey(getch()));
    ManageInfoByState(game_info);
  }

  EndGame(game_info);

  endwin();
}