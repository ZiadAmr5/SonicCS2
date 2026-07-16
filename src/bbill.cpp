#include "bbill.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

BBill::BBill()
{
    setRect(0, 0, 40, 20);
    setBrush(Qt::black);
    setPen(Qt::NoPen);
}

void BBill::update()
{
    moveBy(-speed, 0);

    // Delete once it leaves the left side of the level
    if (scene() && sceneBoundingRect().right() < scene()->sceneRect().left())
    {
        scene()->removeItem(this);
        delete this;
        return;
    }
}