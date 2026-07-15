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

    float acceleration = 900.0f;
    float friction     =1800.0f;
    float gravity      = 0.45f * 60 * 60*1.5;
    float jumpingGravity= 0.12f * 60 * 60*1.5;

    float maxSpeedWalk = 90.0f;
    float maxSpeedRun =   300.0f;
    float maxTerminalVelocity= 5.5f * 60;
    float skidDeceleration = 0.125f * 60 * 60;
    float deltatime;
    float scaleFactor; // Unused for now


    int        lives = 3;
    int        coins = 0;
    int        score = 0;
    PowerState power = PowerState::Small;

    // --- Health / invincibility (survive two hits) ---
    int  maxHealth        = 2;
    bool isInvincible     = false;
    int  invincibleFrames = 0;   // remaining i-frames after taking a hit

    int    facing   = 1;         // last horizontal direction (+1 right, -1 left), for fireballs
    double animTime = 0.0;       // advances with distance travelled; drives the walk cycle

    int currentFrame() const;    // 0 idle | 1-3 walk | 4 jump

public:
    Player(QGraphicsItem* parent = nullptr);
   // void update() { void Player::physUpdate(bool right, bool left, bool jumped, bool jumpHeld,bool runButtonPressed);}

    // Draws the Mario sprite (Small/Big/Fire) instead of a plain rectangle.
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

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

    // --- UML gameplay behaviour (Mario) ---
    void stompEnemy(Enemy* e);
    void collectCoin(Coin* c);
    void powerUp(PowerState newState);
    void addScore(int points);

    void takeDamage(int amount = 1) override;   // lose health, with brief i-frames
    void tickInvincibility();                   // count down i-frames (call once per frame)
    bool isDead() const { return health <= 0; }

    void loseLife()    { if (lives > 0) lives--; }                 // on death, spend a life
    void resetHealth() { health = maxHealth; isInvincible = false; invincibleFrames = 0; }
    void resetPower()  { powerUp(PowerState::Small); }             // back to Small Mario

    PowerState getPower()  const { return power; }
    bool       canShoot()  const { return power == PowerState::Fire; }
    int        getFacing() const { return facing; }

    int  getLives()     const { return lives; }
    int  getCoins()     const { return coins; }
    int  getScore()     const { return score; }
    void setMaxHealth(int value) {maxHealth=value;}
    int  getMaxHealth() const { return maxHealth; }
    bool getInvincible() const { return isInvincible; }
};

#endif // PLAYER_H
