#include "PowerUp.h"
#include "player.h"
#include "Sprites.h"
#include <QPainter>

PowerUp::PowerUp(PowerType t, QGraphicsItem* parent) : GameObject(parent), type(t) {}

void PowerUp::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    const QPixmap& a = Sprites::atlas();
    if (a.isNull()) { // atlas missing -> fall back to the plain box
        painter->setBrush(brush());
        painter->setPen(pen());
        painter->drawRect(rect());
        return;
    }
    const int col = (type == PowerType::Mushroom) ? Sprites::ITEM_MUSHROOM : Sprites::ITEM_FLOWER;
    painter->drawPixmap(rect(), a, Sprites::cell(col, Sprites::ROW_ITEM));
}

void PowerUp::apply(Player* p)
{
    if (!p) return;
    // Mushroom -> Big Mario, Fire Flower -> Fire Mario (can shoot fireballs).
    p->powerUp(type == PowerType::Mushroom ? PowerState::Big : PowerState::Fire);
}
