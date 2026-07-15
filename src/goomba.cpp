#include "goomba.h"
#include "Sprites.h"
#include <QPainter>

Goomba::Goomba(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    // Hitbox is one tile; the sprite is drawn over it in paint().
    setRect(0, 0, 40, 40);

    // Fallback colour if the atlas is missing
    setBrush(Qt::green);
    setPen(Qt::NoPen);
    movingRight = true;
    speed = 2.0;

    timer = 0;
    patrolFrames = 100;      // 3 seconds at 60 FPS
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
    const int frame = (animTimer / 12) % 2 ? Sprites::GOOMBA_2 : Sprites::GOOMBA_1;
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

void Goomba::update()
{
    timer++;
    animTimer++;

    if(timer >= patrolFrames)
    {
        movingRight = !movingRight;
        timer = 0;
    }

    if(movingRight)
    {
        moveBy(speed, 0);
    }
    else
    {
        moveBy(-speed, 0);
    }
}
//
