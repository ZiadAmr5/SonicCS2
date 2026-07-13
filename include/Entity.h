#ifndef ENTITY_H
#define ENTITY_H

#include "GameObject.h"

// Any object with health that can move and take damage (Player, Enemy). SCAFFOLD.
class Entity : public GameObject
{
protected:
    int health = 1;

public:
    explicit Entity(QGraphicsItem* parent = nullptr);

    virtual void takeDamage(int amount = 1);
    virtual void move();
    virtual bool checkCollision(GameObject* other);

    int getHealth() const { return health; }
};

#endif // ENTITY_H
