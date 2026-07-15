#ifndef GOOMBA_H
#define GOOMBA_H

#include "Enemy.h"

// A Goomba.  UML: Goomba -> Enemy -> Entity -> GameObject.
// All the walking/gravity/ledge logic lives in Enemy; this class only adds the
// NES sprite (2-frame waddle) and its own tuning.
class Goomba : public Enemy
{
public:
    explicit Goomba(QGraphicsItem *parent = nullptr);

    void update() override;   // animate, then run the inherited patrol AI
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    int animTimer = 0;        // drives the 2-frame waddle
};

#endif // GOOMBA_H
