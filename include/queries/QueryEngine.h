#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include "structures/HashEncadeamento.h"
#include "structures/BTree.h"
#include "entities/Player.h"
#include "entities/Game.h"
#include "entities/PurchasedGame.h"
#include <vector>
#include <string>
#include <algorithm>

class QueryEngine {
public:
    QueryEngine(HashEncadeamento* playerHash, BTree* achievementTree, 
               const std::vector<Game>* games,
               const std::vector<PurchasedGame>* purchases)
        : hash(playerHash), btree(achievementTree), 
          allGames(games), allPurchases(purchases) {}

    // Consulta 1: Busca por jogador (ID ou nickname)
    Player* searchPlayer(const std::string& identifier) {
        try {
            int id = std::stoi(identifier);
            return hash->searchById(id);
        } catch (...) {
            return hash->searchByNick(identifier);
        }
    }

    // Consulta 2: Jogos de um jogador
    std::vector<Game> getPlayerGames(int playerId) {
        std::vector<Game> result;
        Player* player = hash->searchById(playerId);
        
        if (!player) return result;

        for (const auto& purchase : *allPurchases) {
            if (purchase.getPlayerId() == playerId) {
                auto it = std::find_if(allGames->begin(), allGames->end(),
                    [&](const Game& g) { return g.getId() == purchase.getGameId(); });
                
                if (it != allGames->end()) {
                    result.push_back(*it);
                }
            }
        }
        return result;
    }

    // Consulta 3: Top N jogadores por conquistas
    std::vector<std::pair<Player*, int>> getTopPlayers(int n) {
        std::vector<std::pair<Player*, int>> topPlayers;
        std::vector<int> playerIds = btree->getTopAchievements(n);
        
        for (int id : playerIds) {
            Player* p = hash->searchById(id);
            if (p) {
                int count = btree->getAchievementCount(id);
                topPlayers.emplace_back(p, count);
            }
        }
        return topPlayers;
    }

    // Consulta 4: Jogadores por intervalo de conquistas
    std::vector<std::pair<Player*, int>> getPlayersByAchievementRange(int min, int max) {
        std::vector<std::pair<Player*, int>> result;
        std::vector<int> playerIds = btree->searchRange(min, max);
        
        for (int id : playerIds) {
            Player* p = hash->searchById(id);
            if (p) {
                result.emplace_back(p, btree->getAchievementCount(id));
            }
        }
        return result;
    }

private:
    HashEncadeamento* hash;
    BTree* btree;
    const std::vector<Game>* allGames;
    const std::vector<PurchasedGame>* allPurchases;
};

#endif