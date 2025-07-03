#ifndef GAME_H
#define GAME_H

#include <string>

class Game {
public:
    Game(int id, const std::string& title, const std::string& developers, const std::string& genres)
        : gameId(id), gameTitle(title), gameDevelopers(developers), gameGenres(genres) {}

    // Métodos implementados inline
    int getId() const { return gameId; }
    const std::string& getTitle() const { return gameTitle; }
    const std::string& getDevelopers() const { return gameDevelopers; }
    const std::string& getGenres() const { return gameGenres; }

private:
    int gameId;
    std::string gameTitle;
    std::string gameDevelopers;
    std::string gameGenres;
};

#endif