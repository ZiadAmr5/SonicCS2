#include "GameObject.h"

GameObject::GameObject(QGraphicsItem* parent) : QGraphicsRectItem(parent) {}

void GameObject::update()      { /* TODO: advance object state one frame */ }
void GameObject::draw()        { /* TODO: Qt paints via the scene; hook custom drawing here */ }
QRectF GameObject::getBounds() { return boundingRect(); }
void GameObject::animate()     { /* TODO: advance animation frame */ }
