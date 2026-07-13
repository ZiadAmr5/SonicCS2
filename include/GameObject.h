#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsRectItem>
#include <QRectF>

// Base class for every in-game object (player, enemies, coins, blocks, hazards...).
// Inherits QGraphicsRectItem so each object drops straight into the existing QGraphicsScene,
// exactly like the current Player does. SCAFFOLD: members mirror the Mario UML; bodies are stubs.
class GameObject : public QGraphicsRectItem
{
protected:
    int    objectID    = 0;
    double velocityX   = 0.0;
    double velocityY   = 0.0;
    double acceleration = 0.0;
    int    width       = 0;
    int    height      = 0;
    bool   isActive    = true;

public:
    explicit GameObject(QGraphicsItem* parent = nullptr);
    virtual ~GameObject() = default;

    // Per-frame hooks. Virtual (not pure) so subclasses stay instantiable while scaffolding.
    virtual void   update();
    virtual void   draw();
    virtual QRectF getBounds();
    virtual void   animate();

    int  getObjectID() const { return objectID; }
    bool getIsActive() const { return isActive; }
    void setIsActive(bool active) { isActive = active; }
};

#endif // GAMEOBJECT_H
