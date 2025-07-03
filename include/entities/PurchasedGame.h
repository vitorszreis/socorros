#ifndef PURCHASEDGAME_H
#define PURCHASEDGAME_H

class PurchasedGame {
public:
    PurchasedGame(int playerId, int gameId)
        : playerId(playerId), gameId(gameId) {}

    // Métodos implementados inline
    int getPlayerId() const { return playerId; }
    int getGameId() const { return gameId; }

private:
    int playerId;
    int gameId;
};

#endif