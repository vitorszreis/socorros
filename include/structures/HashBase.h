#ifndef HASHBASE_H
#define HASHBASE_H

#include <string>
#include <vector>
#include "Player.h"

// O uso de funções virtuais puras (= 0) garante que qualquer classe filha
// seja obrigada a implementar todas estas funções.
class HashBase {
public:
    // O destrutor virtual é essencial para a herança correta.
    virtual ~HashBase() = default;

    // Funções de manipulação de dados
    virtual void insert(const Player& player) = 0;
    virtual Player* searchById(int playerId) = 0;
    virtual Player* searchByNick(const std::string& nickname) = 0;
    virtual bool removeById(int playerId) = 0;
    virtual bool removeByNick(const std::string& nickname) = 0;
    virtual void clear() = 0;

    // Funções de utilidade e estatísticas
    virtual void printStats() = 0;
    virtual int getTamanhoTabela() const = 0;
    virtual int getNumElementos() const = 0;
    virtual std::vector<Player> getAllPlayers() const = 0;
};

#endif 
