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

    // Draws the NES Goomba sprite (2-frame waddle) instead of a plain rectangle.
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    bool movingRight;
    double speed;
    int timer;
    int patrolFrames;
    int animTimer = 0;   // drives the 2-frame waddle
};


#endif // GOOMBA_H