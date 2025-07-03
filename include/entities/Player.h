#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "data/SteamIdMapper.h" // Inclui o mapper completo

class Player {
private:
    int internalId;
    std::string steamId;
    std::string nickname;
    std::string country;

public:
    // Construtor inline
    Player(const std::string& steamId, const std::string& nickname, const std::string& country)
        : internalId(SteamIdMapper::getInternalId(steamId)),
          steamId(steamId),
          nickname(nickname),
          country(country) {}

    // Métodos de acesso
    int getId() const { return internalId; }
    const std::string& getSteamId() const { return steamId; }
    const std::string& getNickname() const { return nickname; }
    const std::string& getCountry() const { return country; }
};

#endif