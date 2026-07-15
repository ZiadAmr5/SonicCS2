#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <QString>

class Player;

// Base class for every enemy.  UML: Enemy -> Entity -> GameObject.
//
// It owns the shared "walk the level" behaviour so any subclass (Goomba, and later
// e.g. Bullet Bill) gets it for free: gravity, landing on solid blocks, turning
// around at walls, and turning around at ledges instead of walking into pits.
// Subclasses supply their own look (paint) and tuning.
class Enemy : public Entity
{
public:
    explicit Enemy(QGraphicsItem* parent = nullptr);

    virtual void update();            // one frame of AI - called by the game loop
    virtual void stomped();           // jumped on from above -> die
    virtual void hitPlayer(Player* p); // side contact -> hurt the player
    void         destroy();

    int getDirection() const { return direction; }

protected:
    void patrol();                    // walk + gravity + wall/ledge turning

    QString enemyType;
    double  speed     = 1.2;          // px per frame
    int     direction = -1;           // -1 = left, +1 = right
    // NOTE: vertical speed uses GameObject::velocityY (inherited).
};

#endif // ENEMY_H
