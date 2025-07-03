#ifndef STEAMDATALOADER_H
#define STEAMDATALOADER_H

#include <string>
#include <vector>
#include "../entities/Player.h"
#include "../entities/Game.h"
#include "../entities/Achievement.h"
#include "../entities/History.h"
#include "../entities/Price.h"
#include "../entities/PurchasedGame.h"

using namespace std;

class SteamDataLoader {
public:
    static vector<Player> loadPlayers(const string& filepath);
    static vector<Game> loadGames(const string& filepath);
    static vector<Achievement> loadAchievements(const string& filepath);
    static vector<History> loadHistory(const string& filepath);
    static vector<Price> loadPrices(const string& filepath);
    static vector<PurchasedGame> loadPurchasedGames(const string& filepath);

private:
    static string trim(const string& str); // Função auxiliar para limpeza de strings
};

#endif
