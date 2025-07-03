#ifndef PRICE_H
#define PRICE_H

class Price {
public:
    Price(int gameId, double price)
        : gameId(gameId), price(price) {}

    // Métodos implementados inline
    int getGameId() const { return gameId; }
    double getPrice() const { return price; }

private:
    int gameId;
    double price;
};

#endif