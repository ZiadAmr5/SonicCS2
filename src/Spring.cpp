#include "Spring.h"
#include "player.h"

Spring::Spring(QGraphicsItem* parent) : GameObject(parent) {}

void Spring::launch(Player* p) { Q_UNUSED(p); /* TODO: set player vertical speed = -launchForce */ }
