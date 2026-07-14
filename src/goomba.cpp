#include "goomba.h"

Goomba::Goomba(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    // Temporary rectangle until you add a sprite
    setRect(0, 0, 40, 40);

    // Brown color (Goomba placeholder)
    setBrush(Qt::green);
    movingRight = true;
    speed = 2.0;

    timer = 0;
    patrolFrames = 100;      // 3 seconds at 60 FPS
}

void Goomba::update()
{
    timer++;

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
