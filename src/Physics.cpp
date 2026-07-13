#include "Physics.h"
#include "GameObject.h"

void Physics::applyGravity(GameObject* obj)  { Q_UNUSED(obj); /* TODO: velocityY += gravity * dt */ }
void Physics::applyVelocity(GameObject* obj) { Q_UNUSED(obj); /* TODO: pos += velocity * dt */ }
bool Physics::checkCollision(GameObject* a, GameObject* b) { Q_UNUSED(a); Q_UNUSED(b); return false; }
