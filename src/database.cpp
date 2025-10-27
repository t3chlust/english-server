#include <mariadb/conncpp.hpp>
#include <mariadb/conncpp/PreparedStatement.hpp>
#include <memory>

#include "database.h"
#include "utils.h"

Database::Database() {
    sql::Driver* driver = sql::mariadb::get_driver_instance();
    sql::SQLString url("jdbc:mariadb://localhost:3306/english");
    sql::Properties properties({{"user", "root"}, {"password", ""}});
    conn = std::unique_ptr<sql::Connection>(driver->connect(url, properties));
    if (!conn) {
        printf("Invalid database connection\n");
        exit(0);
    }
    std::shared_ptr<sql::Statement> stmnt(conn->createStatement());
    stmnt->executeQuery(
        "CREATE TABLE IF NOT EXISTS Word (id int unsigned primary key "
        "auto_increment, "
        "chat_id int unsigned not null, word varchar(50) not null, translation "
        "varchar(50) not null, submitted int unsigned not null, "
        "unique(chat_id, word))");
}

void Database::addWord(uint32_t chat_id, sql::SQLString word,
                       sql::SQLString translation, uint32_t submitted) {
    std::shared_ptr<sql::PreparedStatement> stmnt(
        conn->prepareStatement("INSERT INTO test.Word VALUES (?, ?, ?, ?)"));
    stmnt->setUInt(1, chat_id);
    stmnt->setString(2, word);
    stmnt->setString(3, translation);
    stmnt->setUInt(4, submitted);
    stmnt->executeUpdate();
}

int Database::getWordDeletionCount(uint32_t chat_id, sql::SQLString word) {
    std::shared_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement(
        "SELECT submitted FROM test.Word WHERE chat_id = ? AND word = ?"));
    stmnt->setUInt(1, chat_id);
    stmnt->setString(2, word);
    std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
    while (res->next()) {
        return res->getInt("submitted");
    }
    return -1;
}

void Database::incrementWordDeletionCount(uint32_t chat_id,
                                          sql::SQLString word) {
    std::shared_ptr<sql::PreparedStatement> stmnt(
        conn->prepareStatement("UPDATE test.Word SET submitted = submitted "
                               "- 1 WHERE chat_id = ? AND word = ?"));
    stmnt->setUInt(1, chat_id);
    stmnt->setString(2, word);
    stmnt->executeQuery();
}

void Database::deleteWord(uint32_t chat_id, sql::SQLString word) {
    std::shared_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement(
        "DELETE FROM test.Word WHERE chat_id = ? AND word = ?"));
    stmnt->setUInt(1, chat_id);
    stmnt->setString(2, word);
    stmnt->executeUpdate();
}

bool Database::existsWord(sql::SQLString word) {
    std::shared_ptr<sql::PreparedStatement> stmnt(
        conn->prepareStatement("SELECT EXISTS (SELECT * FROM test.Word "
                               "WHERE word = ?) as word_exists"));
    stmnt->setString(1, word);
    std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
    while (res->next()) {
        return res->getBoolean("word_exists");
    }
    return false;
}

std::vector<Word> Database::getActualWords() {
    std::shared_ptr<sql::Statement> stmnt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> res(
        stmnt->executeQuery("SELECT * FROM english.Word"));
    std::vector<Word> result;
    while (res->next()) {
        result.push_back(Word{res->getUInt("id"), res->getUInt("chat_id"),
                              res->getString("word").c_str(),
                              res->getString("translation").c_str(),
                              res->getUInt("submitted")});
    }
    return result;
}
