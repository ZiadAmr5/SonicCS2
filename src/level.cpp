#include "level.h"

void level::loadFromUiScene(QGraphicsScene* scene)
{
    // Clear out any old level memory
    collisionBoundaries.clear();

    // 1. Set world bounds to match whatever dimensions you gave the UI scene
    this->widthpx = scene->sceneRect().width();
    this->lengthpx = scene->sceneRect().height();

    // 2. Loop through every single object painted on your UI workspace canvas
    for (QGraphicsItem* item : scene->items())
    {
        // 3. For Mario, look for standard rectangle blocks
        if (QGraphicsRectItem* rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item))
        {
            // Optional: Skip the background image or the player rectangle itself!
            if (rectItem->data(0).toString() == "player") continue;

            collisionBoundary boundary;
            boundary.shape = collisionBoundary::Rectangle;

            // Map the item's local coordinates to the true scene space coordinates
            QRectF sceneRect = rectItem->mapToScene(rectItem->rect()).boundingRect();

            boundary.x = sceneRect.x();
            boundary.y = sceneRect.y();
            boundary.w = sceneRect.width();
            boundary.l = sceneRect.height();

            // Store properties from the UI editor's property sheets if needed
            boundary.name = rectItem->data(1).toString().isEmpty() ? "solid_block" : rectItem->data(1).toString();

            // Push it into your existing vector array
            collisionBoundaries.append(boundary);
        }
    }
}
