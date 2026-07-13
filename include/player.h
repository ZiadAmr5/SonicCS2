#ifndef PLAYER_H
#define PLAYER_H

#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QtMath>
#include <QPointF>
#include "Entity.h"

class Enemy;
class Coin;

// Mario power state (UML: Player.power). Small -> Big -> Fire.
enum class PowerState { Small, Big, Fire };

// UML: Player -> Entity -> GameObject -> QGraphicsRectItem.
// The existing Mario physics below is unchanged; the UML gameplay members are added on top.
class Player : public Entity
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

    // --- UML gameplay state (Mario) — not yet wired into the loop ---
    int        lives = 3;
    int        coins = 0;
    int        score = 0;
    PowerState power = PowerState::Small;

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

    // --- UML gameplay behaviour (Mario) — stubs to be wired in ---
    void stompEnemy(Enemy* e);
    void collectCoin(Coin* c);
    void powerUp(PowerState newState);
    void addScore(int points);

    int  getLives() const { return lives; }
    int  getCoins() const { return coins; }
    int  getScore() const { return score; }
    void takeDamage();
    bool invincible = false;
    int invincibleFrames = 0;
};

#endif // PLAYER_H
