#include "game_upd_stat.h"

void UpdateScore(GameInfo_t *game_info, int count_line_cleared) {
  int score = 0;

  switch (count_line_cleared) {
    case 1:
      score = 100;
      break;

    case 2:
      score = 300;
      break;

    case 3:
      score = 700;
      break;

    default:
      score = 1500;
      break;
  }

  game_info->total_line_cleared += count_line_cleared;
  game_info->score += score;

  if (game_info->score > game_info->high_score)
    game_info->high_score = game_info->score;
}

void UpdateLevel(GameInfo_t *game_info) {
  if (game_info->level < (MAX_LEVELS - 1)) {
    int last_level = game_info->level;

    game_info->level += (int)((game_info->score - game_info->last_saved_score) /
                              UPD_LEVEL_SCORE);

    if (last_level != game_info->level)
      game_info->last_saved_score = game_info->score;
  }
}