#include "player_name.h"

void EnterPlayerName(GameInfo_t *game_info) {
  mvprintw(2, 10, "ENTER YOUR NAME: ");
  refresh();

  getnstr(game_info->player, 9);
  refresh();

  for (int i = 0; i < (int)strlen(game_info->player); i++) {
    game_info->player[i] = toupper((unsigned char)game_info->player[i]);
  }
}