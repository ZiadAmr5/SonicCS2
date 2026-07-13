#include "Entity.h"

Entity::Entity(QGraphicsItem* parent) : GameObject(parent) {}

void Entity::takeDamage(int amount)            { Q_UNUSED(amount); /* TODO: subtract health, i-frames */ }
void Entity::move()                            { /* TODO: apply velocity to position */ }
bool Entity::checkCollision(GameObject* other) { Q_UNUSED(other); return false; /* TODO */ }
