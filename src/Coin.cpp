#include "Coin.h"
#include "player.h"

Coin::Coin(QGraphicsItem *parent)
    : GameObject(parent)
{}

void Coin::collect(Player *p)
{
    Q_UNUSED(p);
    collected = true;
    setIsActive(false); /* TODO: p->collectCoin(this) */
}
