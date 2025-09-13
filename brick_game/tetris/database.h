#ifndef SRC_GUI_CLI_DB_H
#define SRC_GUI_CLI_DB_H

#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#define SCORE_TABLE_ROWS 10

typedef struct {
  int id[SCORE_TABLE_ROWS];
  char player_name[SCORE_TABLE_ROWS][10];
  int score[SCORE_TABLE_ROWS];
  int rows_exec;
} TableHighScoresTop10;

void InitDataBase();

void AddPlayerAndScore(const char *name, int score);

void UpdateScoreOfPlayer(const char *name, int new_score);

int GetSinglePlayerScore(const char *name, int *score);

void GetTopPlayers(TableHighScoresTop10 *data);

void CloseDataBase();

#endif