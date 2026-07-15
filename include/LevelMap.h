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
#include "PowerUp.h"
#include "LevelData.h"
#include "Sprites.h"
#include "bbill.h"

// Spawns a LevelData's tile map into the scene: solid block per 'X', coin per 'C',
// Goomba per 'N', goal per 'E', player at 'P'; sizes the world to fit the map.
// Call this before the level's loadFromUiScene() so it picks up the solid blocks
// as collision boundaries. Edit the maps themselves in Levels.h.
inline void buildLevel(QGraphicsScene* scene, Player* player, const LevelData& lvl, double tile = 40.0)
{
    const QColor groundFill(150, 90, 45);
    const QColor groundEdge(90, 54, 27);
    // SMB3 "Expanded Wood" ground: a surface tile where the block is exposed to the sky,
    // plain wood fill for anything buried underneath.
    const QPixmap woodTop  = Sprites::woodTile(true,  int(tile));
    const QPixmap woodFill = Sprites::woodTile(false, int(tile));
    // Floating platforms use the NES brick so they read as platforms, not terrain.
    const QPixmap brickTex = Sprites::tile(Sprites::ITEM_BRICK, Sprites::ROW_ITEM, int(tile));

    const QStringList& map = lvl.map;
    const int rows = map.size();
    int cols = 0;
    for (const QString& row : map)
        cols = qMax(cols, row.length());

    // --- Work out which solid blocks belong to the ground mass ---
    // Flood-fill from the bottom row: anything connected to it is terrain (wood);
    // anything left over is a floating platform (brick).
    auto isX = [&](int r, int c) -> bool {
        if (r < 0 || r >= rows || c < 0) return false;
        const QString& s = map.at(r);
        return c < s.length() && s.at(c) == QLatin1Char('X');
    };
    QVector<bool> grounded(rows * cols, false);
    QVector<int>  stack;
    for (int c = 0; c < cols; ++c)
        if (isX(rows - 1, c)) { grounded[(rows - 1) * cols + c] = true; stack.push_back((rows - 1) * cols + c); }
    while (!stack.isEmpty())
    {
        const int idx = stack.takeLast();
        const int r = idx / cols, c = idx % cols;
        const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
        for (int k = 0; k < 4; ++k)
        {
            const int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            const int ni = nr * cols + nc;
            if (grounded[ni] || !isX(nr, nc)) continue;
            grounded[ni] = true;
            stack.push_back(ni);
        }
    }

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

                QPixmap tex;
                if (grounded[r * cols + c]) {
                    // Terrain: wood. Surface tile if nothing solid sits directly above.
                    tex = isX(r - 1, c) ? woodFill : woodTop;
                } else {
                    tex = brickTex;  // floating platform -> brick
                }

                if (!tex.isNull()) {
                    block->setBrush(QBrush(tex));
                    block->setPen(Qt::NoPen);
                } else {                            // fallback: flat brown
                    block->setBrush(QBrush(groundFill));
                    block->setPen(QPen(groundEdge, 1));
                }
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
            else if (ch == QLatin1Char('M') || ch == QLatin1Char('F'))
            {
                const bool mushroom = (ch == QLatin1Char('M'));
                const double s = tile * 0.7;
                auto* pu = new PowerUp(mushroom ? PowerUp::PowerType::Mushroom
                                                : PowerUp::PowerType::FireFlower);
                pu->setRect(0, 0, s, s);
                pu->setPos(c * tile + (tile - s) / 2.0, r * tile + (tile - s) / 2.0);
                pu->setBrush(QBrush(mushroom ? QColor(220, 50, 50)    // red mushroom
                                             : QColor(255, 140, 0))); // orange flower
                pu->setPen(QPen(QColor(255, 255, 255), 2));
                pu->setData(0, QStringLiteral("powerup"));
                scene->addItem(pu);
            }
            else if (ch == QLatin1Char('P') && player)
            {
                player->setPos(c * tile, r * tile);
            }
            else if (ch == QLatin1Char('B'))
            {
                auto *b = new BBill();

                b->setPos(c * tile, r * tile);
                b->setData(0, QStringLiteral("enemy"));

                scene->addItem(b);
            }
        }
    }
}

#endif // LEVELMAP_H
