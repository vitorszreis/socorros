#ifndef PURCHASEDGAME_H
#define PURCHASEDGAME_H

// Esta classe associa um jogador (pelo ID) a um jogo (pelo ID) que ele comprou.

class PurchasedGame {
public:
    // Construtor da classe PurchasedGame.
    PurchasedGame(int playerId, int gameId);

    // Métodos para obter os atributos do jogo comprado.
    int getPlayerId() const;
    int getGameId() const;

private:
    int playerId; // ID do jogador que comprou o jogo.
    int gameId;   // ID do jogo comprado.
};

#endif 
