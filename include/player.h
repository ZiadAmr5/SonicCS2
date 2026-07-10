#ifndef PLAYER_H
#define PLAYER_H
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <cmath>
class Player:public QGraphicsRectItem
{

    bool jumped;
    float velocityX;
    float velocityY;
   float acceleration= 2.8125*60;
    float deceleration=0.5*60*60;
    float friction=0.046875*60;
    float deltatime;
    float scaleFactor;
    float MaxSpeed = 6*60;


public:
    Player(QGraphicsItem* parent = nullptr);
    void physUpdate(bool right ,bool left ,bool jump);
    //void keyPressEvent(QKeyEvent* event);
    float getHorizontalSpeed()
    {
        return velocityX;
    }
    float getVerticalSpeed()
    {
        return velocityY;
    }
    void setDeltaTime(double dt)
    {
        deltatime =dt;
    }
    void getScaleFactor(float sf)
    {
        scaleFactor=sf;
    }

};
template <class T>
T sgn(T value)
{
    return value>0? 1:-1;
}

#endif // PLAYER_H
