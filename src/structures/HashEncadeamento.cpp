#include "../../include/structures/HashEncadeamento.h"
#include <iostream>
#include <functional>
#include <vector>

/**
 * @brief Construtor que inicializa as tabelas com um tamanho fornecido.
 */
HashEncadeamento::HashEncadeamento(int tamanho) : tamanhoTabela(tamanho), numElementos(0), numColisoes(0) {
    tabelaId.resize(tamanho);
    tabelaNick.resize(tamanho);
}

/**
 * @brief Destrutor que limpa a memória.
 */
HashEncadeamento::~HashEncadeamento() {
    clear();
}

/**
 * @brief Função de hash para ID (inteiro).
 */
int HashEncadeamento::hashId(int id) const {
    return id % tamanhoTabela;
}

/**
 * @brief Função de hash para nickname (string).
 */
int HashEncadeamento::hashNick(const std::string& nick) const {
    return std::hash<std::string>{}(nick) % tamanhoTabela;
}

/**
 * @brief Redimensiona a tabela quando o fator de carga excede o limite.
 */
void HashEncadeamento::resize() {
    // Coleta todos os ponteiros de jogadores existentes
    std::vector<Player*> all_players;
    for(const auto& list : tabelaId) {
        for(Player* p : list) {
            all_players.push_back(p);
        }
    }

    // Dobra o tamanho e limpa as tabelas antigas (sem deletar os ponteiros ainda)
    tamanhoTabela *= 2;
    tabelaId.assign(tamanhoTabela, std::list<Player*>());
    tabelaNick.assign(tamanhoTabela, std::list<Player*>());
    numElementos = 0;
    numColisoes = 0;

    // Reinsere todos os jogadores nas novas tabelas.
    // A função 'insert' vai criar CÓPIAS dos jogadores.
    for (const auto* p : all_players) {
        insert(*p);
    }

    // Libera a memória dos objetos Player originais
    for (Player* p : all_players) {
        delete p;
    }
}

/**
 * @brief Insere um jogador, verificando a necessidade de redimensionamento.
 */
void HashEncadeamento::insert(const Player& player) {
    // Fator de carga > 0.7 é um bom gatilho para redimensionar
    if (static_cast<double>(numElementos) / tamanhoTabela > 0.7) {
        resize();
    }
    
    Player* newPlayer = new Player(player);

    // Inserção na tabela por ID
    int hash_id = hashId(newPlayer->getId());
    if (!tabelaId[hash_id].empty()) {
        numColisoes++;
    }
    tabelaId[hash_id].push_back(newPlayer);

    // Inserção na tabela por Nickname
    int hash_nick = hashNick(newPlayer->getNickname());
    if (!tabelaNick[hash_nick].empty()) {
        numColisoes++;
    }
    tabelaNick[hash_nick].push_back(newPlayer);

    numElementos++;
}

/**
 * @brief Busca um jogador pelo seu ID.
 */
Player* HashEncadeamento::searchById(int playerId) {
    int hash_val = hashId(playerId);
    for (Player* p : tabelaId[hash_val]) {
        if (p->getId() == playerId) {
            return p;
        }
    }
    return nullptr;
}

/**
 * @brief Busca um jogador pelo seu nickname.
 */
Player* HashEncadeamento::searchByNick(const std::string& nickname) {
    int hash_val = hashNick(nickname);
    for (Player* p : tabelaNick[hash_val]) {
        if (p->getNickname() == nickname) {
            return p;
        }
    }
    return nullptr;
}

/**
 * @brief Remove um jogador por ID.
 */
bool HashEncadeamento::removeById(int playerId) {
    Player* player_to_remove = searchById(playerId);
    if (!player_to_remove) {
        return false;
    }
    // Para manter a consistência, a remoção por Nickname lida com a exclusão do objeto.
    return removeByNick(player_to_remove->getNickname());
}

/**
 * @brief Remove um jogador por nickname, limpando de ambas as tabelas.
 */
bool HashEncadeamento::removeByNick(const std::string& nickname) {
    int hash_nick = hashNick(nickname);
    Player* player_ptr = nullptr;

    // Procura e remove da lista de nicknames
    for (auto it = tabelaNick[hash_nick].begin(); it != tabelaNick[hash_nick].end(); ++it) {
        if ((*it)->getNickname() == nickname) {
            player_ptr = *it;
            tabelaNick[hash_nick].erase(it);
            break;
        }
    }

    if (!player_ptr) return false;

    // Procura e remove da lista de IDs
    int hash_id = hashId(player_ptr->getId());
    for (auto it = tabelaId[hash_id].begin(); it != tabelaId[hash_id].end(); ++it) {
        if ((*it)->getId() == player_ptr->getId()) {
            tabelaId[hash_id].erase(it);
            break;
        }
    }

    delete player_ptr; // Libera a memória
    numElementos--;
    return true;
}

/**
 * @brief Limpa completamente as tabelas.
 */
void HashEncadeamento::clear() {
    for (auto& list : tabelaId) {
        for (Player* p : list) {
            delete p;
        }
        list.clear();
    }
    for (auto& list : tabelaNick) {
        list.clear();
    }
    numElementos = 0;
    numColisoes = 0;
}

/**
 * @brief Imprime as estatísticas da tabela.
 */
void HashEncadeamento::printStats() {
    std::cout << "--- Estatisticas (Encadeamento) ---\n";
    std::cout << "Tamanho: " << tamanhoTabela << "\n";
    std::cout << "Elementos: " << numElementos << "\n";
    std::cout << "Fator de Carga: " << static_cast<double>(numElementos) / tamanhoTabela << "\n";
    std::cout << "Colisoes: " << numColisoes << "\n";
    std::cout << "-----------------------------------\n";
}

int HashEncadeamento::getTamanhoTabela() const { return tamanhoTabela; }
int HashEncadeamento::getNumElementos() const { return numElementos; }

std::vector<Player> HashEncadeamento::getAllPlayers() const {
    std::vector<Player> allPlayers;
    allPlayers.reserve(numElementos);
    for (const auto& list : tabelaId) {
        for (const Player* p : list) {
            allPlayers.push_back(*p);
        }
    }
    return allPlayers;
}
