#ifndef LEVELMAP_H
#define LEVELMAP_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QStringList>
#include "player.h"
#include "Endpoint.h"

// =============================================================================
//  ====>  EDIT YOUR LEVEL HERE  <====
//
//  Each line below is one row of the level, from top to bottom.
//    'X' = solid block (ground / platform / wall)
//    'P' = player start position (only the first 'P' is used)
//    'E' = end goal - touching it finishes the level (stack several for a tall flag)
//    ' ' = empty space (sky)
//
//  - One character = one TILE (default 40 px). The player is 40x60 (~1.5 tiles).
//  - Rows may be different lengths; the world width is the longest row.
//  - Add/remove rows freely; the world height grows to match.
//  - Run (Z) to build speed before jumping the 2-wide pits.
//  Recompile (Build & Run in Qt Creator) to see your changes.
// =============================================================================
static const QStringList LEVEL_MAP = {
    "                                                                                                    ", // 0  sky
    "                                                                                                    ", // 1
    "                                                                                                    ", // 2
    "                              XXXXXXX                                         XXXXXXX               ", // 3  high platforms
    "                                                                                                    ", // 4
    "            XXXXX                       XXXX              XXXXX                         XXXXX       ", // 5  platforms
    "                 XXXX                                             XXXX                              ", // 6
    "                     XXXXX                                              XXXXX                       ", // 7  mid platforms
    "                                                                                                    ", // 8
    "   P                                                                                           E    ", // 9  player start / goal flag
    "XXXXXXXXX                    XXXXXXXX                         XXXXXXXX                         E    ", // 10 hills + blocks
    "XXXXXXXXXXXXXXXXX                    XXXXXXXX                                 XXXXXXXX         E    ", // 11 goal on the ground
    "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXX", // 12 ground (two pits at cols 24-25 and 72-73)
    "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXX", // 13 ground
    "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXX", // 14 ground
};

// Builds LEVEL_MAP into the scene: one solid block per 'X', a goal per 'E',
// places the player at 'P', and sizes the world to fit the map. Call this before
// the level's loadFromUiScene() so it picks the blocks up as collision boundaries.
inline void buildLevel(QGraphicsScene* scene, Player* player, double tile = 40.0)
{
    const QColor groundFill(150, 90, 45);
    const QColor groundEdge(90, 54, 27);

    const int rows = LEVEL_MAP.size();
    int cols = 0;
    for (const QString& row : LEVEL_MAP)
        cols = qMax(cols, row.length());

    // World size = map size, so the camera clamps to the level edges.
    scene->setSceneRect(0, 0, cols * tile, rows * tile);

    // Tag the player so level::loadFromUiScene skips it (it isn't a wall).
    if (player)
        player->setData(0, QStringLiteral("player"));

    for (int r = 0; r < rows; ++r)
    {
        const QString& row = LEVEL_MAP.at(r);
        for (int c = 0; c < row.length(); ++c)
        {
            const QChar ch = row.at(c);
            if (ch == QLatin1Char('X'))
            {
                auto* block = new QGraphicsRectItem(0, 0, tile, tile);
                block->setPos(c * tile, r * tile);
                block->setBrush(QBrush(groundFill));
                block->setPen(QPen(groundEdge, 1));
                block->setData(0, QStringLiteral("solid"));
                scene->addItem(block);
            }
            else if (ch == QLatin1Char('E'))
            {
                auto* goal = new Endpoint();
                goal->setRect(0, 0, tile, tile);
                goal->setPos(c * tile, r * tile);
                goal->setBrush(QBrush(QColor(40, 180, 70)));  // green goal
                goal->setPen(QPen(QColor(20, 110, 40), 2));
                goal->setData(0, QStringLiteral("endpoint"));
                scene->addItem(goal);
            }
            else if (ch == QLatin1Char('P') && player)
            {
                player->setPos(c * tile, r * tile);
            }
        }
    }
}

#endif // LEVELMAP_H
