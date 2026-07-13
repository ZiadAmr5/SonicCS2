#ifndef HAZARD_H
#define HAZARD_H

#include "GameObject.h"

class Player;

// Spikes / lava / pit — damages the player on contact (replaces Sonic's Spike). SCAFFOLD.
class Hazard : public GameObject
{
    int damage = 1;

public:
    explicit Hazard(QGraphicsItem* parent = nullptr);

    void hurt(Player* p);
};

#endif // HAZARD_H
