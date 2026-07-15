#ifndef SPRITES_H
#define SPRITES_H

#include <QPixmap>
#include <QRect>

// Shared access to the NES-style sprite atlas (assets/nes_atlas.png), built from the
// "Mario (Super Mario Bros. 3 NES-Style)" sheet. 16x16 cells, 4 columns:
//
//   row 0:  goomba walk 1 | goomba walk 2 | goomba squashed |
//   row 1:  coin spin 1   | coin spin 2   | coin spin 3     | coin spin 4
//   row 2:  mushroom      | fire flower   | brick           | ? block
//
namespace Sprites
{
inline const QPixmap& atlas()
{
    static const QPixmap p(QStringLiteral(":/assets/assets/nes_atlas.png"));
    return p;
}

// Source rect of a 16x16 cell in the atlas.
inline QRect cell(int col, int row) { return QRect(col * 16, row * 16, 16, 16); }

// A cell scaled up to `size` px, ready to use as a brush/pixmap (nearest-neighbour = crisp pixels).
inline QPixmap tile(int col, int row, int size = 40)
{
    const QPixmap& a = atlas();
    if (a.isNull()) return QPixmap();
    return a.copy(cell(col, row)).scaled(size, size, Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

// Atlas cell indices
enum { GOOMBA_1 = 0, GOOMBA_2 = 1, GOOMBA_FLAT = 2 };
enum { ROW_ENEMY = 0, ROW_COIN = 1, ROW_ITEM = 2 };
enum { ITEM_MUSHROOM = 0, ITEM_FLOWER = 1, ITEM_BRICK = 2, ITEM_QBLOCK = 3 };

// --- Ground: SMB3 "Expanded Wood" tileset (assets/wood_tiles.png) ---
// Two 16x16 cells: [0] = top surface (with the highlight edge), [1] = plain fill.
inline const QPixmap& woodSheet()
{
    static const QPixmap p(QStringLiteral(":/assets/assets/wood_tiles.png"));
    return p;
}

// `top` = the block's surface is exposed (nothing solid directly above it).
inline QPixmap woodTile(bool top, int size = 40)
{
    const QPixmap& w = woodSheet();
    if (w.isNull()) return QPixmap();
    return w.copy(top ? 0 : 16, 0, 16, 16).scaled(size, size, Qt::IgnoreAspectRatio, Qt::FastTransformation);
}
}

#endif // SPRITES_H
