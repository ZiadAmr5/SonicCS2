#ifndef LEVELMAP_H
#define LEVELMAP_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QStringList>
#include "player.h"
#include "Endpoint.h"
#include "Coin.h"
#include "goomba.h"

// =============================================================================
//  ====>  EDIT YOUR LEVEL HERE  <====
//
//  Each line below is one row of the level, from top to bottom.
//    'X' = solid block (ground / platform / wall)
//    'P' = player start position (only the first 'P' is used)
//    'E' = end goal - touching it finishes the level (stack several for a tall flag)
//    'C' = coin  (collect for health / score; 100 coins = +1 life)
//    'N' = eNemy (Goomba - stomp from above to kill, side contact hurts you)
//    ' ' = empty space (sky)
//
//  - One character = one TILE (default 40 px). The player is 40x60 (~1.5 tiles).
//  - Rows may be different lengths; the world width is the longest row.
//  - Keep the bottom ground rows full width - they define how far the world extends.
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
    "XXXXXXXXX                    XXXXXXXX         CCCCCCC         XXXXXXXX                         E    ", // 10 hills + blocks + coins
    "XXXXXXXXXXXXXXXXX   N   CCCCC        XXXXXXXX CCCCCCC     N                   XXXXXXXX         E    ", // 11 goal + coins (C) + enemies (N)
    "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXX", // 12 ground (two pits at cols 24-25 and 72-73)
    "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXX", // 13 ground
    "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXX", // 14 ground
};

// Builds LEVEL_MAP into the scene: solid block per 'X', coin per 'C', Goomba per 'N',
// goal per 'E', player at 'P'; sizes the world to fit the map. Call before the level's
// loadFromUiScene() so it picks up the solid blocks as collision boundaries.
inline void buildLevel(QGraphicsScene* scene, Player* player, double tile = 40.0)
{
    const QColor groundFill(150, 90, 45);
    const QColor groundEdge(90, 54, 27);

    const int rows = LEVEL_MAP.size();
    int cols = 0;
    for (const QString& row : LEVEL_MAP)
        cols = qMax(cols, row.length());

    scene->setSceneRect(0, 0, cols * tile, rows * tile);

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
                goal->setBrush(QBrush(QColor(40, 180, 70)));
                goal->setPen(QPen(QColor(20, 110, 40), 2));
                goal->setData(0, QStringLiteral("endpoint"));
                scene->addItem(goal);
            }
            else if (ch == QLatin1Char('C'))
            {
                const double s = tile * 0.5;
                auto* coin = new Coin();
                coin->setRect(0, 0, s, s);
                coin->setPos(c * tile + (tile - s) / 2.0, r * tile + (tile - s) / 2.0);
                coin->setBrush(QBrush(QColor(255, 215, 0)));
                coin->setPen(QPen(QColor(150, 110, 0), 1));
                coin->setData(0, QStringLiteral("coin"));
                scene->addItem(coin);
            }
            else if (ch == QLatin1Char('N'))
            {
                auto* g = new Goomba(); // 40x40, patrols left/right
                g->setPos(c * tile, r * tile);
                g->setData(0, QStringLiteral("enemy"));
                scene->addItem(g);
            }
            else if (ch == QLatin1Char('P') && player)
            {
                player->setPos(c * tile, r * tile);
            }
        }
    }
}

#endif // LEVELMAP_H
