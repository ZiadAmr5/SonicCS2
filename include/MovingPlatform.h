#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "GameObject.h"

// Platform that oscillates along a path. SCAFFOLD.
class MovingPlatform : public GameObject
{
    double speed    = 0.0;
    double distance = 0.0;

public:
    explicit MovingPlatform(QGraphicsItem* parent = nullptr);

    void movePlatform();
};

#endif // MOVINGPLATFORM_H
