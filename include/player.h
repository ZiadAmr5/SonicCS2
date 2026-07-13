#ifndef PLAYER_H
#define PLAYER_H

#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QtMath>
#include <QPointF>

class Player : public QGraphicsRectItem
{
private:
    bool isOnGround = false;
    bool isJumping = false;

    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float groundSpeed = 0.0f;

    float acceleration = 0.0375f * 60 * 60;
    float friction     =0.05f * 60 * 60;
    float gravity      = 0.1166f * 60 * 60;

    float maxSpeedWalk = 90.0f;
    float maxSpeedRun =   150.0f;
    float maxTerminalVelocity =4.0f * 60;
    float skidDeceleration = 0.125f * 60 * 60;
    float deltatime;
    float scaleFactor; // Unused for now

public:
    Player(QGraphicsItem* parent = nullptr);


    void physUpdate(bool right, bool left, bool jumped, bool jumpHeld,bool runButtonPressed);

    float getHorizontalSpeed() const { return velocityX; }
    float getVerticalSpeed()   const { return velocityY; }
    bool getIsOnGround()       const { return isOnGround; }

    void setDeltaTime(double dt)   { deltatime = dt; }
    void getScaleFactor(float sf)  { scaleFactor = sf; }

    void setHorizontalSpeed(float value) { velocityX = value; }
    void setVerticalSpeed(float value)   { velocityY = value; }
    void setGroundSpeed(float value)     { groundSpeed = value; }
    void setIsOnGround(bool value)       { isOnGround = value; }
};

#endif // PLAYER_H
