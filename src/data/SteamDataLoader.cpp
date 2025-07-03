#include "SteamDataLoader.h"
#include "SteamIdMapper.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream> // Para std::cerr
#include <sstream>  // Para stringstream
#include <algorithm> // Para remove

using namespace std;

// Converte SteamID para int (função auxiliar)
static int steamIdToInt32(const std::string& steamId) {
    try {
        return std::stoi(steamId);
    } catch (...) {
        return -1; // Ou outro valor padrão para IDs inválidos
    }
}

// Extrai IDs de uma string no formato [1,2,3]
static std::vector<int> extractIdsFromList(const std::string& listStr) {
    std::vector<int> ids;
    std::stringstream ss(listStr.substr(1, listStr.size() - 2)); // Remove os colchetes
    std::string item;
    
    while (std::getline(ss, item, ',')) {
        try {
            ids.push_back(std::stoi(item));
        } catch (...) {
            // Ignora IDs inválidos
        }
    }
    return ids;
}


vector<Player> SteamDataLoader::loadPlayers(const string& filepath) {
    vector<Player> players;
    ifstream file(filepath);
    
    if (!file.is_open()) {
        throw runtime_error("Erro ao abrir arquivo: " + filepath);
    }

    string line;
    getline(file, line); // Pula cabeçalho

    while (getline(file, line)) {
        stringstream ss(line);
        string steamId, nickname, country;
        
        getline(ss, steamId, ',');
        getline(ss, nickname, ',');
        getline(ss, country);

        // Correção: Operações de limpeza DEVEM estar dentro do bloco while
        nickname.erase(remove(nickname.begin(), nickname.end(), '\"'), nickname.end());
        nickname = trim(nickname);
        
        country.erase(remove(country.begin(), country.end(), '\"'), country.end());
        country = trim(country);

        players.emplace_back(steamId, nickname, country);
    }
    return players;
}


vector<Game> SteamDataLoader::loadGames(const string& filepath) {
    vector<Game> games;
    ifstream file(filepath);
    
    if (!file.is_open()) {
        throw runtime_error("Erro ao abrir arquivo: " + filepath);
    }

    string line;
    getline(file, line); // Pula cabeçalho

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, title, developers, genres;
        
        getline(ss, idStr, ',');
        getline(ss, title, ',');
        getline(ss, developers, ',');
        getline(ss, genres);

        // Limpeza dos dados
        title = trim(title);
        developers = trim(developers);
        genres = trim(genres);

        try {
            int gameId = stoi(idStr);
            games.emplace_back(gameId, title, developers, genres);
        } catch (const invalid_argument& e) {
            cerr << "Erro: ID de jogo inválido '" << idStr << "' na linha: " << line << endl;
            continue;
        } catch (const out_of_range& e) {
            cerr << "Erro: ID de jogo fora do intervalo '" << idStr << "' na linha: " << line << endl;
            continue;
        }
    }
    return games;
}

vector<Achievement> SteamDataLoader::loadAchievements(const string& filepath) {
    vector<Achievement> achievements;
    ifstream file(filepath);
    
    if (!file.is_open()) {
        throw runtime_error("Erro ao abrir arquivo: " + filepath);
    }

    string line;
    getline(file, line); // Pula cabeçalho

    while (getline(file, line)) {
        stringstream ss(line);
        string gameIdStr, achievementName;
        
        getline(ss, gameIdStr, ',');
        getline(ss, achievementName);

        achievementName = trim(achievementName);

        try {
            int gameId = stoi(gameIdStr);
            achievements.emplace_back(gameId, achievementName);
        } catch (const invalid_argument& e) {
            cerr << "Erro: ID de jogo inválido '" << gameIdStr << "' na linha: " << line << endl;
            continue;
        } catch (const out_of_range& e) {
            cerr << "Erro: ID de jogo fora do intervalo '" << gameIdStr << "' na linha: " << line << endl;
            continue;
        }
    }
    return achievements;
}

vector<History> SteamDataLoader::loadHistory(const string& filepath) {
    vector<History> history;
    ifstream file(filepath);
    
    if (!file.is_open()) {
        throw runtime_error("Erro ao abrir arquivo: " + filepath);
    }

    string line;
    getline(file, line); // Pula cabeçalho

    while (getline(file, line)) {
        stringstream ss(line);
        string playerIdStr, achievementName;
        
        getline(ss, playerIdStr, ',');
        getline(ss, achievementName);

        achievementName = trim(achievementName);

        try {
            long long steamId = stoll(playerIdStr);
            int playerId = static_cast<int>(steamId & 0xFFFFFFFF);
            history.emplace_back(playerId, achievementName);
        } catch (const invalid_argument& e) {
            cerr << "Erro: ID de jogador inválido '" << playerIdStr << "' na linha: " << line << endl;
            continue;
        } catch (const out_of_range& e) {
            cerr << "Erro: ID de jogador muito grande '" << playerIdStr << "' na linha: " << line << endl;
            continue;
        }
    }
    return history;
}

vector<Price> SteamDataLoader::loadPrices(const string& filepath) {
    vector<Price> prices;
    ifstream file(filepath);
    
    if (!file.is_open()) {
        throw runtime_error("Erro ao abrir arquivo: " + filepath);
    }

    string line;
    getline(file, line); // Pula cabeçalho

    while (getline(file, line)) {
        stringstream ss(line);
        string gameIdStr, priceStr;
        
        getline(ss, gameIdStr, ',');
        getline(ss, priceStr);

        try {
            int gameId = stoi(gameIdStr);
            double price = stod(priceStr);
            prices.emplace_back(gameId, price);
        } catch (const invalid_argument& e) {
            cerr << "Erro: Dados inválidos na linha: " << line << endl;
            continue;
        } catch (const out_of_range& e) {
            cerr << "Erro: Dados fora do intervalo na linha: " << line << endl;
            continue;
        }
    }
    return prices;
}

vector<PurchasedGame> SteamDataLoader::loadPurchasedGames(const string& filepath) {
    vector<PurchasedGame> purchases;
    ifstream file(filepath);
    
    if (!file.is_open()) {
        throw runtime_error("Erro ao abrir arquivo: " + filepath);
    }

    string line;
    getline(file, line);

    int lineNum = 1;
    while (getline(file, line)) {
        lineNum++;
        stringstream ss(line);
        string playerIdStr, gamesListStr;
        
        getline(ss, playerIdStr, ',');
        getline(ss, gamesListStr);

        playerIdStr = trim(playerIdStr);
        gamesListStr = trim(gamesListStr);

        if (playerIdStr.empty()) {
            cerr << "Aviso [Linha " << lineNum << "]: PlayerID vazio - linha ignorada" << endl;
            continue;
        }

        int playerId = steamIdToInt32(playerIdStr);

        try {
            if (!gamesListStr.empty() && gamesListStr.front() == '[' && gamesListStr.back() == ']') {
                vector<int> gameIds = extractIdsFromList(gamesListStr);
                for (int gameId : gameIds) {
                    purchases.emplace_back(playerId, gameId);
                }
            } else {
                int gameId = stoi(gamesListStr);
                purchases.emplace_back(playerId, gameId);
            }
        } catch (const exception& e) {
            cerr << "Erro [Linha " << lineNum << "]: " << e.what() << " - linha ignorada" << endl;
        }
    }
    return purchases;
}


// Implementação da função trim
string SteamDataLoader::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\"");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r\"");
    return str.substr(first, (last - first + 1));
}
