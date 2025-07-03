#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <unordered_map> 
#include "data/SteamDataLoader.h"
#include "data/SaveManager.h"
#include "queries/QueryEngine.h"
#include "structures/BTree.h"
#include "structures/HashEncadeamento.h"

using namespace std;

// --- Protótipos de Funções ---
void displayMenu();
void clearInputBuffer();
void displayPlayerDetails(const Player* player);
void displayPlayerList(const vector<pair<Player*, int>>& players);
void displayGameList(const vector<Game>& games);

// --- Funções de Display ---
void displayMenu() {
    cout << "\n=== SISTEMA DE ANÁLISE DE PERFIS ===" << endl;
    cout << "1. 🔍 Buscar jogador por ID/nickname" << endl;
    cout << "2. 🎮 Listar jogos de um jogador" << endl;
    cout << "3. 🏆 Listar Top N jogadores (conquistas)" << endl;
    cout << "4. 📊 Listar jogadores por faixa de conquistas" << endl;
    cout << "5. 🚪 Sair" << endl;
    cout << "Escolha uma opção: ";
}

void displayPlayerDetails(const Player* player) {
    if (!player) {
        cout << "\n❌ Jogador não encontrado!\n";
        return;
    }

    cout << "\n=== DETALHES DO JOGADOR ===" << endl;
    cout << left << setw(15) << "ID:" << player->getId() << endl;
    cout << setw(15) << "Nickname:" << player->getNickname() << endl;
    cout << setw(15) << "País:" << player->getCountry() << endl;
    cout << string(30, '-') << endl;
}

void displayPlayerList(const vector<pair<Player*, int>>& players) {
    if (players.empty()) {
        cout << "\n❌ Nenhum jogador encontrado!\n";
        return;
    }

    cout << "\n=== LISTA DE JOGADORES ===" << endl;
    cout << left << setw(10) << "ID" 
              << setw(20) << "Nickname" 
              << setw(15) << "País"
              << setw(12) << "Conquistas" << endl;
    cout << string(57, '-') << endl;

    for (const auto& [player, count] : players) {
        cout << setw(10) << player->getId() 
             << setw(20) << player->getNickname() 
             << setw(15) << player->getCountry()
             << setw(12) << count << endl;
    }
    cout << string(57, '-') << endl;
    cout << "Total: " << players.size() << " jogadores\n";
}

void displayGameList(const vector<Game>& games) {
    if (games.empty()) {
        cout << "\n❌ Nenhum jogo encontrado!\n";
        return;
    }

    cout << "\n=== LISTA DE JOGOS ===" << endl;
    cout << left << setw(30) << "Título" 
              << setw(20) << "Gênero" 
              << setw(25) << "Desenvolvedor" << endl;
    cout << string(75, '-') << endl;

    for (const auto& game : games) {
        cout << setw(30) << game.getTitle() 
             << setw(20) << game.getGenres() 
             << setw(25) << game.getDevelopers() << endl;
    }
    cout << string(75, '-') << endl;
    cout << "Total: " << games.size() << " jogos\n";
}

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- Função Principal ---
int main() {
    // Inicialização
    cout << "⏳ Inicializando o sistema...\n";
    
    const string btreeFile = "achievements.btree";
    const string hashFile = "players.hash";
    const std::string dataPath = "/mnt/c/Users/User/Desktop/UFJF/TrabalhoED2/trab01-soja-ti/data/csv";
    // Carrega ou cria estruturas
    BTree* achievementTree = SaveManager::loadBTree(btreeFile);
    HashEncadeamento* playerHash = new HashEncadeamento(10000);
    
    vector<Game> games;
    vector<PurchasedGame> purchases;
    vector<Player> players;

    if (!achievementTree) {
        cout << "🔍 Nenhum dado pré-processado encontrado. Carregando CSVs...\n";
        
        delete achievementTree;
        achievementTree = new BTree(3);

        // Carrega dados
        players = SteamDataLoader::loadPlayers(dataPath + "/players.csv");
        games = SteamDataLoader::loadGames(dataPath + "/games.csv");
        purchases = SteamDataLoader::loadPurchasedGames(dataPath + "/purchased_games.csv");
        auto histories = SteamDataLoader::loadHistory(dataPath + "/history.csv");

        // Popula estruturas
        for (const auto& player : players) {
            playerHash->insert(player);
        }

        // Processa conquistas
        unordered_map<int, int> achievementCounts;
        for (const auto& h : histories) {
            achievementCounts[h.getPlayerId()]++;
        }
        for (const auto& [id, count] : achievementCounts) {
            achievementTree->insert(count, id);
        }

        SaveManager::saveBTree(achievementTree, btreeFile);
        cout << "💾 Dados salvos para futuras execuções\n";
    } else {
        cout << "✅ Dados carregados do cache\n";
    }

    // Cria motor de consultas
    QueryEngine queryEngine(playerHash, achievementTree, &games, &purchases);

    // Interface do usuário
    int choice;
    while (true) {
        displayMenu();
        cin >> choice;
        clearInputBuffer();

        try {
            switch (choice) {
                case 1: { // Busca por jogador
                    string input;
                    cout << "Digite ID ou nickname: ";
                    getline(cin, input);
                    Player* player = queryEngine.searchPlayer(input);
                    displayPlayerDetails(player);
                    break;
                }
                case 2: { // Jogos de um jogador
                    cout << "Digite o ID do jogador: ";
                    int id;
                    cin >> id;
                    clearInputBuffer();
                    auto games = queryEngine.getPlayerGames(id);
                    displayGameList(games);
                    break;
                }
                case 3: { // Top N jogadores
                    cout << "Digite o valor de N: ";
                    int n;
                    cin >> n;
                    clearInputBuffer();
                    auto topPlayers = queryEngine.getTopPlayers(n);
                    displayPlayerList(topPlayers);
                    break;
                }
                case 4: { // Jogadores por faixa
                    cout << "Digite o mínimo de conquistas: ";
                    int min;
                    cin >> min;
                    cout << "Digite o máximo de conquistas: ";
                    int max;
                    cin >> max;
                    clearInputBuffer();
                    auto players = queryEngine.getPlayersByAchievementRange(min, max);
                    displayPlayerList(players);
                    break;
                }
                case 5: { // Sair
                    delete achievementTree;
                    delete playerHash;
                    cout << "👋 Encerrando o sistema...\n";
                    return 0;
                }
                default:
                    cout << "❌ Opção inválida!\n";
            }
        } catch (const exception& e) {
            cerr << "💀 Erro: " << e.what() << endl;
            clearInputBuffer();
        }
    }
}