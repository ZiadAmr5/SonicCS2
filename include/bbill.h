#ifndef BBILL_H
#define BBILL_H

#include <QGraphicsRectItem>
#include <Enemy.h>

class BBill : public Enemy
{
public:
    BBill();

    void update();

private:
    double speed = 4.0;
};

#endif
