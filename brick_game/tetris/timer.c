#include "timer.h"

clock_t getAdjustedTime(GameInfo_t *game_info) {
  if (!game_info) return 0;

  if (game_info->pause) {
    return game_info->pauseStartTime - game_info->accumulatedPauseTime;
  }
  return clock() - game_info->accumulatedPauseTime;
}

void PauseGameForTimer(GameInfo_t *game_info) {
  if (!game_info) return;

  if (!game_info->pause) {
    game_info->pause = 1;
    game_info->pauseStartTime = clock();
  }
}

void ResumeGameForTimer(GameInfo_t *game_info) {
  if (!game_info) return;

  if (game_info->pause) {
    clock_t pauseDuration = clock() - game_info->pauseStartTime;
    game_info->accumulatedPauseTime += pauseDuration;
    game_info->pause = 0;
  }
}

int ShouldDropPiece(GameInfo_t *game_info, int level_time) {
  if (!game_info) return 0;

  clock_t currentTime = getAdjustedTime(game_info);
  double elapsed =
      ((double)(currentTime - game_info->lastDropTime) * 1000) / CLOCKS_PER_SEC;

  return elapsed >= level_time;
}