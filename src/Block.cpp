#include "Block.h"
#include "player.h"

Block::Block(QGraphicsItem* parent) : GameObject(parent) {}

void Block::hit(Player* p) { Q_UNUSED(p); /* TODO: spawn `contents` / break if isBreakable */ }
