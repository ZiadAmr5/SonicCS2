#include "Enemy.h"
#include "player.h"
#include <QGraphicsScene>

// Per-frame gravity tuning (the loop runs at ~60 FPS).
static const double ENEMY_GRAVITY  = 0.6;
static const double ENEMY_MAX_FALL = 8.0;

static bool isSolid(const QGraphicsItem* it)
{
    return it->data(0).toString() == QLatin1String("solid");
}

Enemy::Enemy(QGraphicsItem *parent)
    : Entity(parent)
{}

void Enemy::update()
{
    patrol();
}

// Shared enemy movement: walk, fall, turn at walls, don't walk off ledges.
void Enemy::patrol()
{
    QGraphicsScene* sc = scene();
    if (!sc) return;

    // --- 1. Walk sideways; turn around if we bump into a solid block ---
    const double dx = direction * speed;
    moveBy(dx, 0);
    for (QGraphicsItem* it : sc->collidingItems(this))
    {
        if (!isSolid(it)) continue;
        moveBy(-dx, 0);          // step back out of the wall
        direction = -direction;  // and about-face
        break;
    }

    // --- 2. Gravity: fall until we land on something solid ---
    velocityY += ENEMY_GRAVITY;
    if (velocityY > ENEMY_MAX_FALL) velocityY = ENEMY_MAX_FALL;
    moveBy(0, velocityY);

    bool onGround = false;
    for (QGraphicsItem* it : sc->collidingItems(this))
    {
        if (!isSolid(it)) continue;
        const QRectF b = it->sceneBoundingRect();
        if (velocityY > 0) {                     // landed on top
            setPos(x(), b.top() - rect().height());
            onGround = true;
        } else if (velocityY < 0) {                 // bonked its head
            setPos(x(), b.bottom());
        }
        velocityY = 0;
        break;
    }

    // --- 3. Ledge check: turn around rather than walk into a pit ---
    if (onGround)
    {
        const QRectF r = sceneBoundingRect();
        const double probeX = (direction > 0) ? r.right() + 2.0 : r.left() - 2.0;
        const QRectF probe(probeX - 1.0, r.bottom() + 1.0, 2.0, 6.0);

        bool groundAhead = false;
        for (QGraphicsItem* it : sc->items(probe))
            if (isSolid(it)) { groundAhead = true; break; }

        if (!groundAhead) direction = -direction;
    }
}

void Enemy::stomped()
{
    health = 0;
    destroy();
}

void Enemy::hitPlayer(Player* p)
{
    if (p) p->takeDamage(1);
}

void Enemy::destroy()
{
    setIsActive(false); // the game loop removes inactive enemies from the scene
}
