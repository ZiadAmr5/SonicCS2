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

    // Draws the spinning NES coin instead of a plain rectangle.
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    bool isCollected() const { return collected; }
    void collect(Player* p);
};

#endif // COIN_H
