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
   float acceleration= 0.046875*60*60; // Sonic originally moved at 6 pixels/frame , converting to pixels/second for qt, we multiply by 60. acceleration would be pixel/frame^2 so we multiply by 60 twice
    float deceleration=0.5*60*60;
    float friction=0.046875*60;
    float deltatime;
    float scaleFactor; //see getScaleFactor
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
    //this function was to be used to scale from the original 320x225 on the genesis, for now it is unused
    void getScaleFactor(float sf)
    {
        scaleFactor=sf;
    }

};
//return a sign
template <class T>
T sgn(T value)
{
    return value>0? 1:-1;
}

#endif // PLAYER_H
