#ifndef PHYSICS_H
#define PHYSICS_H

class GameObject;

// Central physics helper meant to operate on GameObjects.
// SCAFFOLD: the working Mario physics currently lives in Player::physUpdate and the game loop's
// AABB resolver; this class is the UML's intended home for that logic once it's factored out.
// NOTE: to mutate GameObject's protected members later, add setters or friend this class.
class Physics
{
    double gravity    = 0.1166 * 60 * 60; // matches Player's current gravity
    double friction   = 0.05   * 60 * 60;
    double airControl = 0.5;               // air-accel multiplier (Player halves accel mid-air)

public:
    Physics() = default;

    void applyGravity(GameObject* obj);
    void applyVelocity(GameObject* obj);
    bool checkCollision(GameObject* a, GameObject* b);
};

#endif // PHYSICS_H
