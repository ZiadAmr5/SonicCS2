#include "Hazard.h"
#include "player.h"

Hazard::Hazard(QGraphicsItem *parent)
    : GameObject(parent)
{}

void Hazard::hurt(Player *p)
{
    Q_UNUSED(p); /* TODO: p->takeDamage(damage) + knockback */
}
