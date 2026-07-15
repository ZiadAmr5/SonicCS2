#ifndef POWERUP_H
#define POWERUP_H

#include "GameObject.h"

class Player;

// Mario pickup: Super Mushroom (grow) or Fire Flower (shoot fireballs).
// Placed from the level editor ('M' / 'F'); applies its effect on contact.
class PowerUp : public GameObject
{
public:
    enum class PowerType { Mushroom, FireFlower };

    explicit PowerUp(PowerType t = PowerType::Mushroom, QGraphicsItem* parent = nullptr);

    PowerType getType() const { return type; }
    void      apply(Player* p);   // give the player this power-up's effect

private:
    PowerType type;
};

#endif // POWERUP_H
