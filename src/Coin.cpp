#include "Coin.h"
#include <QElapsedTimer>
#include <QPainter>
#include "Sprites.h"
#include "player.h"

Coin::Coin(QGraphicsItem *parent)
    : GameObject(parent)
{}

void Coin::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    const QPixmap &a = Sprites::atlas();
    if (a.isNull()) { // atlas missing -> fall back to the plain box
        painter->setBrush(brush());
        painter->setPen(pen());
        painter->drawRect(rect());
        return;
    }
    // One shared clock so every coin in the level spins in sync.
    static QElapsedTimer clock;
    if (!clock.isValid())
        clock.start();

    // frames 0,1,2,1 give a smooth back-and-forth spin
    static const int seq[4] = {0, 1, 2, 1};
    const int frame = seq[(clock.elapsed() / 120) % 4];

    painter->drawPixmap(rect(), a, Sprites::cell(frame, Sprites::ROW_COIN));
}

void Coin::collect(Player *p)
{
    Q_UNUSED(p);
    collected = true;
    setIsActive(false); /* TODO: p->collectCoin(this) */
}
