#ifndef GOOMBA_H
#define GOOMBA_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <QPen>

class Goomba : public QGraphicsRectItem
{
public:
    Goomba(QGraphicsItem *parent = nullptr);

    void update();

private:
    bool movingRight;
    double speed;
    int timer;
    int patrolFrames;
};


#endif // GOOMBA_H