#ifndef FIREBALL_H
#define FIREBALL_H

#include "GameObject.h"

// Projectile thrown by Fire Mario. Travels horizontally, kills Goombas on contact,
// and despawns when it hits a solid block or runs out of life.
class Fireball : public GameObject
{
public:
    explicit Fireball(double speedX, QGraphicsItem* parent = nullptr);

    void step(double dt);                    // move one frame
    bool expired() const { return life <= 0; }

private:
    double speed;      // px/s, sign = direction
    double life = 2.0; // seconds before it fizzles out
};

#endif // FIREBALL_H
