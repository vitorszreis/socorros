#include "../../include/structures/HashSondagem.h"

#include <iostream>
#include <functional>

HashSondagem::HashSondagem(int tamanho) : tamanhoTabela(tamanho), numElementos(0), numColisoes(0) {
    tabelaId.resize(tamanho);
    tabelaNick.resize(tamanho);
}

HashSondagem::~HashSondagem() {
    clear();
}

/**
 * @brief Função de hash para chaves inteiras (playerID).
 */
int HashSondagem::hashId(int id) const {
    return id % tamanhoTabela;
}

/**
 * @brief Função de hash para chaves do tipo string (nickname).
 */
int HashSondagem::hashNick(const std::string& nick) const {
    return std::hash<std::string>{}(nick) % tamanhoTabela;
}

/**
 * @brief Redimensiona ambas as tabelas hash.
 */
void HashSondagem::resize() {
    std::vector<HashEntry> oldTableId = tabelaId;

    // Guarda os ponteiros dos jogadores antigos para deletar no final
    std::vector<Player*> playersToDelete;
    for(const auto& entry : oldTableId) {
        if (entry.state == EntryState::OCCUPIED) {
            playersToDelete.push_back(entry.player);
        }
    }

    tamanhoTabela *= 2;
    tabelaId.assign(tamanhoTabela, HashEntry());
    tabelaNick.assign(tamanhoTabela, HashEntry());
    
    numElementos = 0;
    numColisoes = 0;

    // Reinsere os jogadores na nova tabela. A função 'insert' criará novas cópias.
    for (Player* player : playersToDelete) {
        if (player) {
            insert(*player);
        }
    }

    // Agora, deleta os ponteiros da tabela antiga
    for (Player* player : playersToDelete) {
        delete player;
    }
}

/**
 * @brief Insere um jogador em ambas as tabelas hash.
 */
void HashSondagem::insert(const Player& player) {
    if (static_cast<double>(numElementos) / tamanhoTabela > 0.7) {
        resize();
    }

    Player* newPlayer = new Player(player);

    // --- Inserção na tabela por ID ---
    int hash_id = hashId(newPlayer->getId());
    int i = 0;
    int id_index;
    while (true) {
        id_index = (hash_id + i) % tamanhoTabela;
        if (tabelaId[id_index].state != EntryState::OCCUPIED) {
            break;
        }
        i++;
        numColisoes++;
    }
    tabelaId[id_index].player = newPlayer;
    tabelaId[id_index].state = EntryState::OCCUPIED;
    
    // --- Inserção na tabela por Nickname ---
    int hash_nick = hashNick(newPlayer->getNickname());
    i = 0;
    int nick_index;
    while (true) {
        nick_index = (hash_nick + i) % tamanhoTabela;
        if (tabelaNick[nick_index].state != EntryState::OCCUPIED) {
            break;
        }
        i++;
        numColisoes++;
    }
    tabelaNick[nick_index].player = newPlayer;
    tabelaNick[nick_index].state = EntryState::OCCUPIED;

    numElementos++;
}

/**
 * @brief Busca um jogador pelo ID.
 */
Player* HashSondagem::searchById(int playerId) {
    int hash_val = hashId(playerId);
    int i = 0;
    while (tabelaId[(hash_val + i) % tamanhoTabela].state != EntryState::EMPTY) {
        auto& entry = tabelaId[(hash_val + i) % tamanhoTabela];
        if (entry.state == EntryState::OCCUPIED && entry.player->getId() == playerId) {
            return entry.player;
        }
        i++;
        if (i >= tamanhoTabela) break; // Evita loop infinito
    }
    return nullptr;
}

/**
 * @brief Busca um jogador pelo nickname.
 */
Player* HashSondagem::searchByNick(const std::string& nickname) {
    int hash_val = hashNick(nickname);
    int i = 0;
    while (tabelaNick[(hash_val + i) % tamanhoTabela].state != EntryState::EMPTY) {
        auto& entry = tabelaNick[(hash_val + i) % tamanhoTabela];
        if (entry.state == EntryState::OCCUPIED && entry.player->getNickname() == nickname) {
            return entry.player;
        }
        i++;
        if (i >= tamanhoTabela) break; // Evita loop infinito
    }
    return nullptr;
}

/**
 * @brief Remove um jogador a partir do seu ID.
 */
bool HashSondagem::removeById(int playerId) {
    Player* player_to_remove = searchById(playerId);
    if (!player_to_remove) return false;
    return removeByNick(player_to_remove->getNickname());
}

/**
 * @brief Remove um jogador a partir do seu nickname.
 */
bool HashSondagem::removeByNick(const std::string& nickname) {
    int hash_nick = hashNick(nickname);
    int i = 0;
    int nick_index = -1;
    while (tabelaNick[(hash_nick + i) % tamanhoTabela].state != EntryState::EMPTY) {
        auto& entry = tabelaNick[(hash_nick + i) % tamanhoTabela];
        if (entry.state == EntryState::OCCUPIED && entry.player->getNickname() == nickname) {
            nick_index = (hash_nick + i) % tamanhoTabela;
            break;
        }
        i++;
        if (i >= tamanhoTabela) break;
    }

    if (nick_index == -1) return false;

    Player* player_ptr = tabelaNick[nick_index].player;
    int id_to_remove = player_ptr->getId();

    int hash_id = hashId(id_to_remove);
    i = 0;
    int id_index = -1;
    while (tabelaId[(hash_id + i) % tamanhoTabela].state != EntryState::EMPTY) {
        auto& entry = tabelaId[(hash_id + i) % tamanhoTabela];
        if (entry.state == EntryState::OCCUPIED && entry.player->getId() == id_to_remove) {
            id_index = (hash_id + i) % tamanhoTabela;
            break;
        }
        i++;
        if (i >= tamanhoTabela) break;
    }
    
    if (id_index == -1) return false;

    delete player_ptr;
    
    // Atribui os membros da struct separadamente.
    tabelaNick[nick_index].player = nullptr;
    tabelaNick[nick_index].state = EntryState::DELETED;
    
    tabelaId[id_index].player = nullptr;
    tabelaId[id_index].state = EntryState::DELETED;

    numElementos--;
    return true;
}

/**
 * @brief Imprime as estatísticas de desempenho da tabela hash.
 */
void HashSondagem::printStats() {
    std::cout << "--- Estatisticas da Tabela Hash (Sondagem Linear) ---\n";
    std::cout << "Tamanho da Tabela: " << tamanhoTabela << "\n";
    std::cout << "Numero de Elementos: " << numElementos << "\n";
    std::cout << "Fator de Carga: " << static_cast<double>(numElementos) / tamanhoTabela << "\n";
    std::cout << "Numero Total de Colisoes na Insercao: " << numColisoes << "\n";
    std::cout << "----------------------------------------------------\n";
}

int HashSondagem::getTamanhoTabela() const { return tamanhoTabela; }
int HashSondagem::getNumElementos() const { return numElementos; }

/**
 * @brief Retorna um vetor com todos os jogadores na tabela.
 */
std::vector<Player> HashSondagem::getAllPlayers() const {
    std::vector<Player> allPlayers;
    allPlayers.reserve(numElementos);
    for (const auto& entry : tabelaId) {
        if (entry.state == EntryState::OCCUPIED) {
            allPlayers.push_back(*entry.player);
        }
    }
    return allPlayers;
}

/**
 * @brief Limpa ambas as tabelas e libera toda a memória alocada.
 */
void HashSondagem::clear() {
    for (auto& entry : tabelaId) {
        if (entry.state == EntryState::OCCUPIED) {
            delete entry.player;
        }
    }
    tabelaId.assign(tamanhoTabela, HashEntry());
    tabelaNick.assign(tamanhoTabela, HashEntry());
    numElementos = 0;
    numColisoes = 0;
}
