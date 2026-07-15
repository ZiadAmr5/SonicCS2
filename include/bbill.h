#ifndef BBILL_H
#define BBILL_H

#include <QGraphicsRectItem>

class BBill : public QGraphicsRectItem
{
public:
    BBill();

    void update();

private:
    double speed = 4.0;
};

#endif