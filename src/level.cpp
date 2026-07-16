#include "level.h"

void level::loadFromUiScene(QGraphicsScene *scene)
{
    // Clear out any old level memory
    collisionBoundaries.clear();

    //  Set world bounds to match dimensions of  the UI scene
    this->widthpx = scene->sceneRect().width();
    this->lengthpx = scene->sceneRect().height();

    //  Loop through every single object painted on the  UI workspace canvas
    for (QGraphicsItem *item : scene->items()) {
        //   look for standard rectangle blocks
        if (QGraphicsRectItem *rectItem = qgraphicsitem_cast<QGraphicsRectItem *>(item)) {
            //dont detect the player
            if (rectItem->data(0).toString() == "player")
                continue;

            collisionBoundary boundary;
            boundary.shape = collisionBoundary::Rectangle;

            QRectF sceneRect = rectItem->mapToScene(rectItem->rect()).boundingRect();

            boundary.x = sceneRect.x();
            boundary.y = sceneRect.y();
            boundary.w = sceneRect.width();
            boundary.l = sceneRect.height();

            boundary.name = rectItem->data(1).toString().isEmpty() ? "solid_block"
                                                                   : rectItem->data(1).toString();

            collisionBoundaries.append(boundary);
        }
    }
}
