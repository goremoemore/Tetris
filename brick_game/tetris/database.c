#include "database.h"

static sqlite3 *db;

void InitDataBase() {
  sqlite3_open("game_scores.db", &db);

  char *sql =
      "CREATE TABLE IF NOT EXISTS scores ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "name TEXT NOT NULL, "
      "score INTEGER NOT NULL);";

  char *err_msg = 0;

  sqlite3_exec(db, sql, 0, 0, &err_msg);
}

void AddPlayerAndScore(const char *name, int score) {
  char *sql = "INSERT INTO scores (name, score) VALUES (?, ?);";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, score);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
}

void UpdateScoreOfPlayer(const char *name, int new_score) {
  char *sql = "UPDATE scores SET score = ? WHERE name = ?;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, new_score);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
}

int GetSinglePlayerScore(const char *name, int *score) {
  char *sql = "SELECT name, score FROM scores WHERE name = ?;";
  sqlite3_stmt *stmt;
  int got = 1;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
      *score = sqlite3_column_int(stmt, 1);
    } else
      got = 0;
    sqlite3_finalize(stmt);
  }

  return got;
}

void GetTopPlayers(TableHighScoresTop10 *data) {
  char *sql =
      "SELECT id, name, score FROM scores ORDER BY score DESC LIMIT 10;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
    int counter = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      int id = sqlite3_column_int(stmt, 0);
      const char *name = (const char *)sqlite3_column_text(stmt, 1);
      int score = sqlite3_column_int(stmt, 2);

      strcpy(data->player_name[counter], name);
      data->score[counter] = score;
      data->id[counter] = id;
      counter++;
    }

    data->rows_exec = counter;
    sqlite3_finalize(stmt);
  }
}

void CloseDataBase() { sqlite3_close(db); }