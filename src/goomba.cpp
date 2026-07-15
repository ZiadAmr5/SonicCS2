#include "goomba.h"
#include "Sprites.h"
#include <QPainter>
#include <QBrush>
#include <QPen>

Goomba::Goomba(QGraphicsItem *parent)
    : Enemy(parent)
{
    // Hitbox is one tile; the sprite is drawn over it in paint().
    setRect(0, 0, 40, 40);

    enemyType = QStringLiteral("goomba");
    speed     = 1.2;
    direction = -1;   // start walking left
    health    = 1;    // one stomp kills it

    // Fallback colour if the atlas is missing
    setBrush(Qt::green);
    setPen(Qt::NoPen);
}

void Goomba::update()
{
    animTimer++;
    Enemy::update();   // gravity + walk + wall/ledge turning
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
    if (direction > 0) { // sprite faces left by default, so mirror when walking right
        painter->translate(rect().center().x(), 0);
        painter->scale(-1, 1);
        painter->translate(-rect().center().x(), 0);
    }
    painter->drawPixmap(rect(), a, src);
    painter->restore();
}
