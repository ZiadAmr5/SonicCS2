#ifndef PLAYER_H
#define PLAYER_H
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <QPen>
class Player:public QGraphicsRectItem
{
    bool leftKeyPressed;
    bool rightKeyPressed;
    bool UpKeyPressed;
    bool DownKeyPressed;
    bool jumped;
    double velocityX;
    double velocityY;
    double accelration;
    int direction;
    double friction=0.04;
    double MaxSpeed = 6.0;

public:
    Player(QGraphicsItem* parent = nullptr);
    void physUpdate(bool right ,bool left ,bool jump);
    void keyPressEvent(QKeyEvent* event);
    double sendHorizontalSpeed(double vx);
};

#endif // PLAYER_H
