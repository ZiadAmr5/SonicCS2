#include "PowerUp.h"
#include "player.h"

PowerUp::PowerUp(PowerType t, QGraphicsItem* parent) : GameObject(parent), type(t) {}

void PowerUp::apply(Player* p)
{
    if (!p) return;
    // Mushroom -> Big Mario, Fire Flower -> Fire Mario (can shoot fireballs).
    p->powerUp(type == PowerType::Mushroom ? PowerState::Big : PowerState::Fire);
}
