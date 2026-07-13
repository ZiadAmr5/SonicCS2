#ifndef ENDPOINT_H
#define ENDPOINT_H

#include "GameObject.h"

class Player;

// Level-finish goal. Placed in the level editor (symbol 'E' in LevelMap.h).
// It is a trigger, not a solid wall: when the player overlaps it, the level ends.
class Endpoint : public GameObject
{
public:
    explicit Endpoint(QGraphicsItem* parent = nullptr);

    // True when the player is touching this endpoint.
    bool isReachedBy(Player* p) const;
};

#endif // ENDPOINT_H
