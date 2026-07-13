#ifndef BLOCK_H
#define BLOCK_H

#include "GameObject.h"
#include <QString>

class Player;

// Solid/? /brick block. Bumped from below by Mario; may break or spawn its contents. SCAFFOLD.
class Block : public GameObject
{
    bool    isBreakable = false;
    QString contents;   // e.g. "coin", "mushroom", "" for a plain solid block

public:
    explicit Block(QGraphicsItem* parent = nullptr);

    void hit(Player* p);
};

#endif // BLOCK_H
