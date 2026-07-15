#include "Enemy.h"
#include "player.h"

Enemy::Enemy(QGraphicsItem *parent)
    : Entity(parent)
{}

void Enemy::patrol()
{ /* TODO: walk between bounds using direction/speed */
}
void Enemy::hitPlayer(Player *p)
{
    Q_UNUSED(p); /* TODO: p->takeDamage() / shrink Mario */
}
void Enemy::stomped()
{ /* TODO: squash animation, then destroy() */
}
void Enemy::destroy()
{
    setIsActive(false); /* TODO: remove from scene */
}
