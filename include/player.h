#ifndef PLAYER_H
#define PLAYER_H
#include <QKeyEvent>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <cmath>
#include "Entity.h"
class Player:public Entity
{

    // --- existing physics state (drives the current working movement) ---
    bool jumped;
    float velocityX;
    float velocityY;
   float acceleration= 0.046875*60*60; // Sonic originally moved at 6 pixels/frame , converting to pixels/second for qt, we multiply by 60. acceleration would be pixel/frame^2 so we multiply by 60 twice
    float deceleration=0.5*60*60;
    float friction=0.046875*60;
    float deltatime;
    float scaleFactor; //see getScaleFactor
    float MaxSpeed = 6*60;

    // --- UML scaffold state (not yet wired into the physics/loop) ---
    int  rings        = 0;
    int  score        = 0;
    int  lives        = 3;
    bool isGrounded   = false;
    bool isonWall     = false;
    bool isonCeiling  = false;
    bool isRolling    = false;
    bool isInvincible = false;


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

    // --- UML scaffold behaviour (stubs) ---
    void moveLeft();
    void moveRight();
    void jump();
    void spinDash();
    void collectRing();
    void loseRing();
    void addScore(int points);

};
//return a sign
template <class T>
T sgn(T value)
{
    return value>0? 1:-1;
}

#endif // PLAYER_H
