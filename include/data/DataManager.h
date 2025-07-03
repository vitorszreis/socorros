#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "HashBase.h"
#include "BTree.h"
#include <unordered_map>
#include <vector>

// Forward declarations para evitar includes desnecessários no header
class History;
class Player;

class DataManager {
public:
    static void buildFromScratch(const std::string& csvPath, HashBase* hash, BTree* tree);
    static void loadAllData(const std::string& basePath, HashBase* hash, BTree* btree);
    
private:
    static std::unordered_map<int, int> countAchievements(const std::vector<History>& histories);
    static void populateStructures(const std::vector<Player>& players,
                                 const std::unordered_map<int, int>& achievements,
                                 HashBase* hash,
                                 BTree* tree);
};

#endif