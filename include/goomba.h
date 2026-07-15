#ifndef GOOMBA_H
#define GOOMBA_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <QPen>

// A Goomba that actually walks the level: it falls with gravity, lands on solid
// blocks, turns around at walls, and turns around at ledges instead of walking
// off into pits.
class Goomba : public QGraphicsRectItem
{
public:
    Goomba(QGraphicsItem *parent = nullptr);

    void update();   // one frame of patrol + gravity + collision

    // Draws the NES Goomba sprite (2-frame waddle) instead of a plain rectangle.
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    bool   movingRight = false;
    double speed       = 1.2;   // px per frame
    double velocityY   = 0.0;   // px per frame (gravity)
    int    animTimer   = 0;     // drives the 2-frame waddle
};

#endif // GOOMBA_H
