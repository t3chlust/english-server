#ifndef DATABASE_H
#define DATABASE_H

#include <mariadb/conncpp.hpp>
#include <unordered_map>

#include "utils.h"

class Database {
  private:
    std::unique_ptr<sql::Connection> conn;

  public:
    Database();

    void addWord(uint32_t chat_id, sql::SQLString word,
                 sql::SQLString translation, int to_delete);

    int getWordDeletionCount(uint32_t chat_id, sql::SQLString word);

    void incrementWordDeletionCount(uint32_t chat_id, sql::SQLString word);

    void deleteWord(uint32_t chat_id, sql::SQLString word);

    bool existsWord(uint32_t chat_id, sql::SQLString word);

    std::vector<Word> getActualWords();
};

#endif // DATABASE_H
