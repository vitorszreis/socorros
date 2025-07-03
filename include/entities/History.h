#ifndef HISTORY_H
#define HISTORY_H

#include <string>

class History {
public:
    // Construtor que recebe playerId (int) diretamente
    History(int playerId, const std::string& achievementName)
        : playerId(playerId), achievementName(achievementName) {}

    // Método para obter o ID do jogador (int)
    int getPlayerId() const { return playerId; }

    const std::string& getAchievementName() const { return achievementName; }

private:
    int playerId;          // ID interno do jogador
    std::string achievementName;
};

#endif