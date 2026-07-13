#ifndef SPRING_H
#define SPRING_H

#include "GameObject.h"

class Player;

// Bounce spring/trampoline. SCAFFOLD.
class Spring : public GameObject
{
    double launchForce = 0.0;

public:
    explicit Spring(QGraphicsItem* parent = nullptr);

    void launch(Player* p);
};

#endif // SPRING_H
