#ifndef HASSONDAGEM_H
#define HASSONDAGEM_H

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <stdexcept>

#include "HashBase.h"
#include "Player.h"

// Enum para o estado de cada slot na tabela hash.
enum class EntryState {
    EMPTY,
    OCCUPIED,
    DELETED
};

// struct para cada entrada da tabela hash.
struct HashEntry {
    Player* player;
    EntryState state;

    HashEntry() : player(nullptr), state(EntryState::EMPTY) {}
};

// Declaração da classe HashSondagem
class HashSondagem : public HashBase {
private:
    std::vector<HashEntry> tabelaId;
    std::vector<HashEntry> tabelaNick;
    int tamanhoTabela;
    int numElementos;
    int numColisoes;

    // Funções de hash privadas
    int hashId(int id) const;
    int hashNick(const std::string& nick) const;
    
    // Função para redimensionar
    void resize();

public:
    explicit HashSondagem(int tamanho);
    ~HashSondagem() override;

    // Funções principais da interface HashBase
    void insert(const Player& player) override;
    Player* searchById(int playerId) override;
    Player* searchByNick(const std::string& nickname) override;
    bool removeById(int playerId) override;
    bool removeByNick(const std::string& nickname) override;
    void clear() override;

    // Funções de estatísticas e utilidade
    void printStats() override;
    int getTamanhoTabela() const override;
    int getNumElementos() const override;
    std::vector<Player> getAllPlayers() const override;
};

#endif // HASSONDAGEM_H
