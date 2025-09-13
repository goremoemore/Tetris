#include "game_process.h"

void GameProcess(GameInfo_t *game_info, int user_action) {
  ManageTetraminoFromTimer(game_info, 0);

  ManageUserAction(game_info, user_action);

  ManageGameState(game_info);
}

static void LineClearActions(GameInfo_t *game_info) {
  if (!game_info->animate_clear_lines_request) CheckLinesForClear(game_info);

  if (!game_info->animate_clear_lines_request)
    game_info->current_state = kSTATE_SPAWN_NEW;

  if (game_info->animate_clear_lines_request &&
      game_info->animated_clear_lines_done) {
    ClearLines(game_info, game_info->start_line_for_animated_clear,
               game_info->count_lines_for_animated_clear);
    ShiftLines(game_info, game_info->start_line_for_animated_clear,
               game_info->count_lines_for_animated_clear);

    UpdateScore(game_info, game_info->count_lines_for_animated_clear);

    UpdateLevel(game_info);

    game_info->animate_clear_lines_request = 0;
    game_info->animated_clear_lines_done = 0;
    game_info->start_line_for_animated_clear = -1;
    game_info->count_lines_for_animated_clear = 0;

    game_info->current_state = kSTATE_SPAWN_NEW;
  }
}

static void NewPlayerActions(GameInfo_t *game_info) {
  int high_score = 0;
  if (GetSinglePlayerScore(game_info->player, &high_score))
    game_info->high_score = high_score;
  else
    AddPlayerAndScore(game_info->player, 0);

  game_info->current_state = kSTATE_MAIN_MENU;
}

static void ShowScoresActions(GameInfo_t *game_info) {
  TableHighScoresTop10 TopPlayers;
  GetTopPlayers(&TopPlayers);
  game_info->show_top_players_request = 1;
  game_info->top_players = TopPlayers;

  game_info->current_state = kSTATE_WAITING_FOR_EXIT;
}

static void StateFirstStartActions(GameInfo_t *game_info) {
  game_info->current_state = kSTATE_SPAWN_NEW;
  game_info->lastDropTime = clock();
}

static void SpawNewTetraminoActions(GameInfo_t *game_info) {
  SpawNewTetramino(game_info);

  if (!CanMoveDownTetramino(game_info, REAL_TETRAMINO)) {
    SetTetraminoToField(game_info, REAL_TETRAMINO);
    game_info->current_state = kSTATE_GAME_OVER;
  } else {
    game_info->current_state = kSTATE_PLAY;
  }
}

static void GameOverActions(GameInfo_t *game_info) {
  game_info->game_over = 1;
  UpdateScoreOfPlayer(game_info->player, game_info->high_score);
  game_info->update_some_info = 1;
}

void ManageGameState(GameInfo_t *game_info) {
  switch (game_info->current_state) {
    case kSTATE_NEW_PLAYER:
      NewPlayerActions(game_info);
      break;

    case kSTATE_SHOW_SCORES:
      ShowScoresActions(game_info);
      break;

    case kSTATE_FIRST_START:
      StateFirstStartActions(game_info);
      break;

    case kSTATE_LINE_CLEAR:
      LineClearActions(game_info);
      break;

    case kSTATE_SPAWN_NEW:
      SpawNewTetraminoActions(game_info);
      break;

    case kSTATE_GAME_OVER:
      GameOverActions(game_info);
      break;

    case kSTATE_PLAY:
    case kSTATE_MAIN_MENU:
    case kSTATE_TERMINATE:
    case kSTATE_PAUSE:
    case kSTATE_WAITING_FOR_EXIT:
      game_info->update_some_info = 1;
      break;
  }
}

void ManageUserAction(GameInfo_t *game_info, int user_action) {
  int is_tetramino_actions_enabled = game_info->current_state == kSTATE_PLAY;

  switch (user_action) {
    case Down:
      if (is_tetramino_actions_enabled) {
        HardDropTetramino(game_info, REAL_TETRAMINO);
        SetTetraminoToField(game_info, REAL_TETRAMINO);
        game_info->update_request = 1;
        game_info->current_state = kSTATE_LINE_CLEAR;
      }
      break;

    case Up:
      break;

    case Left: {
      if (is_tetramino_actions_enabled) {
        int new_x = -1;

        if (CheckToShiftTetramino(game_info, -1, &new_x))
          ShiftTetramino(game_info, new_x);
      }
    } break;

    case Right: {
      if (is_tetramino_actions_enabled) {
        int new_x = -1;

        if (CheckToShiftTetramino(game_info, 1, &new_x))
          ShiftTetramino(game_info, new_x);
      }
    } break;

    case Pause:

      if (game_info->current_state != kSTATE_PAUSE) {
        game_info->current_state = kSTATE_PAUSE;
        PauseGameForTimer(game_info);

      } else {
        ResumeGameForTimer(game_info);

        game_info->current_state = kSTATE_PLAY;
      }

      break;

    case Action:
      if (is_tetramino_actions_enabled) CheckAndRotateTetramino(game_info);
      break;

    case Terminate:
      game_info->current_state = kSTATE_TERMINATE;
      break;

    case ShowScores:
      if (game_info->game_over) game_info->current_state = kSTATE_SHOW_SCORES;
      break;

    case Start:
      if (game_info->current_state == kSTATE_MAIN_MENU)
        game_info->current_state = kSTATE_FIRST_START;
      break;

    default:
      break;
  }
}
