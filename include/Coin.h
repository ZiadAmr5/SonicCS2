#ifndef COIN_H
#define COIN_H

#include "GameObject.h"

class Player;

// Collectible coin (replaces Sonic's Ring). SCAFFOLD.
class Coin : public GameObject
{
    int  value     = 1;
    bool collected = false;

public:
    explicit Coin(QGraphicsItem* parent = nullptr);

    bool isCollected() const { return collected; }
    void collect(Player* p);
};

#endif // COIN_H
