#ifndef LEVELMAP_H
#define LEVELMAP_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include "player.h"
#include "Endpoint.h"
#include "Coin.h"
#include "goomba.h"
#include "LevelData.h"

// Spawns a LevelData's tile map into the scene: solid block per 'X', coin per 'C',
// Goomba per 'N', goal per 'E', player at 'P'; sizes the world to fit the map.
// Call this before the level's loadFromUiScene() so it picks up the solid blocks
// as collision boundaries. Edit the maps themselves in Levels.h.
inline void buildLevel(QGraphicsScene* scene, Player* player, const LevelData& lvl, double tile = 40.0)
{
    const QColor groundFill(150, 90, 45);
    const QColor groundEdge(90, 54, 27);

    const QStringList& map = lvl.map;
    const int rows = map.size();
    int cols = 0;
    for (const QString& row : map)
        cols = qMax(cols, row.length());

    scene->setSceneRect(0, 0, cols * tile, rows * tile);

    if (player)
        player->setData(0, QStringLiteral("player"));

    for (int r = 0; r < rows; ++r)
    {
        const QString& row = map.at(r);
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
