#include "PowerUp.h"
#include "player.h"

PowerUp::PowerUp(QGraphicsItem* parent) : GameObject(parent) {}

void PowerUp::apply(Player* p) { Q_UNUSED(p); /* TODO: p->powerUp(...) based on `type` */ }
