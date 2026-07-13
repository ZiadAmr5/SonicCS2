#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <QString>

class Player;

// Goomba/Koopa-style enemy: killed by a stomp from above, hurts the player on side contact. SCAFFOLD.
class Enemy : public Entity
{
    QString enemyType;
    double  speed     = 0.0;
    int     direction = 1;

public:
    explicit Enemy(QGraphicsItem* parent = nullptr);

    void patrol();
    void hitPlayer(Player* p);   // side contact -> damage Mario
    void stomped();              // jumped on from above -> die
    void destroy();
};

#endif // ENEMY_H
