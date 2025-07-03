// DataLoader.h
#include "SteamDataLoader.h"
#include <vector>

class DataLoader {
public:
    static std::vector<Player> loadPlayers(const std::string& path) {
        return SteamDataLoader::loadPlayers(path);
    }
    
    static std::vector<History> loadHistory(const std::string& path) {
        return SteamDataLoader::loadHistory(path);
    }
};