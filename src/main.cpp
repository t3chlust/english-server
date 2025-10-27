#include "database.h"
#include "utils.h"
#include <httplib.h>
#include <nlohmann/json.hpp>

int main() {
    Database db;

    httplib::Server server;
    server.Get("/actual_words", [&](const httplib::Request&, httplib::Response& res) {
        nlohmann::json j = nlohmann::json::array();
        std::vector<Word> words = db.getActualWords();
        for (const Word &word : words) {
            j.push_back({{"id", word.id},
                         {"chat_id", word.chat_id},
                         {"word", word.word},
                         {"translation", word.translation},
                         {"submitted", word.to_delete}});
        }
        res.set_content(j.dump(), "application/json");
    });
    server.listen("0.0.0.0", 8080);
}
