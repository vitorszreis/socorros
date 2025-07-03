#ifndef HASHENCADEAMENTO_H
#define HASHENCADEAMENTO_H

#include <string>
#include <vector>
#include <list>
#include "HashBase.h"
#include "Player.h"

class HashEncadeamento : public HashBase {
private:
    std::vector<std::list<Player*>> tabelaId;
    std::vector<std::list<Player*>> tabelaNick;
    int tamanhoTabela;
    int numElementos;
    int numColisoes;

    // Funções privadas
    int hashId(int id) const;
    int hashNick(const std::string& nick) const;
    void resize();

public:
    explicit HashEncadeamento(int tamanho);
    ~HashEncadeamento() override;

    // Funções da interface HashBase
    void insert(const Player& player) override;
    Player* searchById(int playerId) override;
    Player* searchByNick(const std::string& nickname) override;
    bool removeById(int playerId) override;
    bool removeByNick(const std::string& nickname) override;
    void clear() override;

    // Funções de estatísticas
    void printStats() override;
    int getTamanhoTabela() const override;
    int getNumElementos() const override;
    std::vector<Player> getAllPlayers() const override;
};

#endif // HASHENCADEAMENTO_H
