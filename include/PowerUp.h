#ifndef POWERUP_H
#define POWERUP_H

#include "GameObject.h"

class Player;

// Mushroom / Fire Flower pickup. SCAFFOLD.
class PowerUp : public GameObject
{
public:
    enum class PowerType { Mushroom, FireFlower };

private:
    PowerType type = PowerType::Mushroom;

public:
    explicit PowerUp(QGraphicsItem* parent = nullptr);

    void apply(Player* p);
};

#endif // POWERUP_H
