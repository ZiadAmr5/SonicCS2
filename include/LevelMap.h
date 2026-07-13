#ifndef LEVELMAP_H
#define LEVELMAP_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QStringList>
#include "player.h"

// =============================================================================
//  ====>  EDIT YOUR LEVEL HERE  <====
//
//  Each line below is one row of the level, from top to bottom.
//    'X' = solid block (ground / platform / wall)
//    'P' = player start position (only the first 'P' is used)
//    ' ' = empty space (sky)
//
//  - One character = one TILE (default 40 px). The player is 40x60 (~1.5 tiles).
//  - Rows may be different lengths; the world width is the longest row.
//  - Add/remove rows freely; the world height grows to match.
//  - To close the pit, just fill the two spaces in the bottom rows with 'X'.
//  Recompile (Build & Run in Qt Creator) to see your changes.
// =============================================================================
static const QStringList LEVEL_MAP = {
    "                                                  ", //  0  sky
    "                                                  ", //  1
    "                                                  ", //  2
    "                              XXXXXXX             ", //  3  high platform
    "                                                  ", //  4
    "            XXXXX                       XXXX      ", //  5  platforms
    "                 XXXX                             ", //  6
    "                     XXXXX                        ", //  7  mid platform
    "                                                  ", //  8
    "   P                                              ", //  9  <- player start
    "XXXXXXXXX                    XXXXXXXX             ", // 10  start hill + block
    "XXXXXXXXXXXXXXXXX                    XXXXXXXX     ", // 11
    "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX", // 12  ground (gap = pit)
    "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX", // 13  ground
    "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX", // 14  ground
};

// Builds LEVEL_MAP into the scene: one solid block per 'X', places the player
// at 'P', and sizes the world to fit the map. Call this before the level's
// loadFromUiScene() so it picks the blocks up as collision boundaries.
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
            else if (ch == QLatin1Char('P') && player)
            {
                player->setPos(c * tile, r * tile);
            }
        }
    }
}

#endif // LEVELMAP_H
