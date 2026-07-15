#include "Endpoint.h"
#include "player.h"

Endpoint::Endpoint(QGraphicsItem *parent)
    : GameObject(parent)
{}

bool Endpoint::isReachedBy(Player *p) const
{
    return p && collidesWithItem(p);
}
