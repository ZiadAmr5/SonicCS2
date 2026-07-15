#include "Fireball.h"

Fireball::Fireball(double speedX, QGraphicsItem* parent) : GameObject(parent), speed(speedX) {}

void Fireball::step(double dt)
{
    moveBy(speed * dt, 0);
    life -= dt;
}
