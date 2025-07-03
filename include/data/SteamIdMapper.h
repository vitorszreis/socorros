#ifndef STEAMIDMAPPER_H
#define STEAMIDMAPPER_H

#include <string>
#include <unordered_map>

class SteamIdMapper {
private:
    // Variáveis estáticas como inline (C++17)
    static inline std::unordered_map<std::string, int> steamToInternal;
    static inline std::unordered_map<int, std::string> internalToSteam;
    static inline int nextId = 1;

public:
    // Implementação completa inline
    static int getInternalId(const std::string& steamId) {
        if (steamId.empty()) return -1;
        
        auto it = steamToInternal.find(steamId);
        if (it == steamToInternal.end()) {
            int newId = nextId++;
            steamToInternal[steamId] = newId;
            internalToSteam[newId] = steamId;
            return newId;
        }
        return it->second;
    }

    static std::string getSteamId(int internalId) {
        auto it = internalToSteam.find(internalId);
        return (it != internalToSteam.end()) ? it->second : "";
    }

    static void clear() {
        steamToInternal.clear();
        internalToSteam.clear();
        nextId = 1;
    }

    static int getNextId() {
        return nextId;
    }
};

#endif