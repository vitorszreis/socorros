#include "DataManager.h"
#include "SteamDataLoader.h"
#include "DataLoader.h"
#include "SaveManager.h"
#include <iostream>
#include <unordered_map>

void DataManager::buildFromScratch(const std::string& csvPath, HashBase* hash, BTree* tree) {
    try {
        std::cout << "⏳ Carregando dados dos CSVs...\n";
        
        auto players = DataLoader::loadPlayers(csvPath + "/players.csv");
        auto histories = DataLoader::loadHistory(csvPath + "/history.csv");
        
        std::cout << "🔢 Processando conquistas...\n";
        auto achievements = countAchievements(histories);
        
        std::cout << "🏗️ Construindo estruturas...\n";
        populateStructures(players, achievements, hash, tree);
        
        std::cout << "💾 Salvando cache binário...\n";
        SaveManager::saveAll(hash, tree);
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Erro durante construção: " << e.what() << std::endl;
        throw;
    }
}

#include "DataManager.h"
#include "SteamIdMapper.h"
#include "SteamDataLoader.h"
#include <iostream>

void DataManager::loadAllData(const std::string& basePath, HashBase* hash, BTree* btree) {
    SteamIdMapper::clear();
    
    auto players = SteamDataLoader::loadPlayers(basePath + "/players.csv");
    auto histories = SteamDataLoader::loadHistory(basePath + "/history.csv");
    
    std::unordered_map<int, int> achievementCounts;
    
    for (const auto& history : histories) {
        // Usando getPlayerId() que retorna int (ID interno)
        int playerId = history.getPlayerId();
        achievementCounts[playerId]++;
    }

    for (const auto& player : players) {
        hash->insert(player);
        
        auto it = achievementCounts.find(player.getId());
        if (it != achievementCounts.end()) {
            btree->insert(it->second, player.getId());
        }
    }
}
std::unordered_map<int, int> DataManager::countAchievements(const std::vector<History>& histories) {
    std::unordered_map<int, int> counts;
    for (const auto& h : histories) {
        counts[h.getPlayerId()]++; // Agora usando o método público
    }
    return counts;
}

void DataManager::populateStructures(const std::vector<Player>& players,
                                   const std::unordered_map<int, int>& achievements,
                                   HashBase* hash,
                                   BTree* tree) {
    for (const auto& player : players) {
        hash->insert(player);
        
        auto it = achievements.find(player.getId());
        if (it != achievements.end()) {
            tree->insert(it->second, player.getId());
        }
    }
}