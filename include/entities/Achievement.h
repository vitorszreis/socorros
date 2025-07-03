#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include <string>

class Achievement {
public:
    Achievement(int gameId, const std::string& name)
        : gameId(gameId), achievementName(name) {}

    // Métodos implementados inline
    int getGameId() const { return gameId; }
    const std::string& getName() const { return achievementName; }

private:
    int gameId;
    std::string achievementName;
};

#endif