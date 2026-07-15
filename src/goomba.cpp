#include "goomba.h"
#include "Sprites.h"
#include <QPainter>
#include <QGraphicsScene>

// Per-frame gravity tuning (the loop runs at ~60 FPS).
static const double GOOMBA_GRAVITY  = 0.6;
static const double GOOMBA_MAX_FALL = 8.0;

static bool isSolid(const QGraphicsItem* it)
{
    return it->data(0).toString() == QLatin1String("solid");
}

Goomba::Goomba(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    // Hitbox is one tile; the sprite is drawn over it in paint().
    setRect(0, 0, 40, 40);

    // Fallback colour if the atlas is missing
    setBrush(Qt::green);
    setPen(Qt::NoPen);
}

void Goomba::update()
{
    QGraphicsScene* sc = scene();
    if (!sc) return;
    animTimer++;

    // --- 1. Walk sideways; turn around if we bump into a solid block ---
    const double dx = movingRight ? speed : -speed;
    moveBy(dx, 0);
    for (QGraphicsItem* it : sc->collidingItems(this))
    {
        if (!isSolid(it)) continue;
        moveBy(-dx, 0);              // step back out of the wall
        movingRight = !movingRight;  // and about-face
        break;
    }

    // --- 2. Gravity: fall until we land on something solid ---
    velocityY += GOOMBA_GRAVITY;
    if (velocityY > GOOMBA_MAX_FALL) velocityY = GOOMBA_MAX_FALL;
    moveBy(0, velocityY);

    bool onGround = false;
    for (QGraphicsItem* it : sc->collidingItems(this))
    {
        if (!isSolid(it)) continue;
        const QRectF b = it->sceneBoundingRect();
        if (velocityY > 0) {                                  // landed on top
            setPos(x(), b.top() - rect().height());
            onGround = true;
        } else if (velocityY < 0) {                           // bonked its head
            setPos(x(), b.bottom());
        }
        velocityY = 0;
        break;
    }

    // --- 3. Ledge check: if there's no ground ahead, turn around instead of
    //        walking off into a pit.
    if (onGround)
    {
        const QRectF r = sceneBoundingRect();
        const double probeX = movingRight ? r.right() + 2.0 : r.left() - 2.0;
        const QRectF probe(probeX - 1.0, r.bottom() + 1.0, 2.0, 6.0);

        bool groundAhead = false;
        for (QGraphicsItem* it : sc->items(probe))
            if (isSolid(it)) { groundAhead = true; break; }

        if (!groundAhead) movingRight = !movingRight;
    }
}

void Goomba::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    const QPixmap& a = Sprites::atlas();
    if (a.isNull()) { // atlas missing -> fall back to the plain box
        painter->setBrush(brush());
        painter->setPen(pen());
        painter->drawRect(rect());
        return;
    }
    // waddle between the two walk frames; face the direction of travel
    const int frame = (animTimer / 10) % 2 ? Sprites::GOOMBA_2 : Sprites::GOOMBA_1;
    const QRect src = Sprites::cell(frame, Sprites::ROW_ENEMY);

    painter->save();
    if (movingRight) { // sprite faces left by default, so mirror when walking right
        painter->translate(rect().center().x(), 0);
        painter->scale(-1, 1);
        painter->translate(-rect().center().x(), 0);
    }
    painter->drawPixmap(rect(), a, src);
    painter->restore();
}
