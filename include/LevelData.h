#ifndef LEVELDATA_H
#define LEVELDATA_H

#include <QString>
#include <QStringList>

// Everything that defines one level: a display name and its tile map.
//
// Tile map symbols (see Levels.h for the actual maps):
//   'X' solid block   'P' player start   'E' goal
//   'C' coin          'N' enemy (Goomba) ' ' empty
//
// Create one LevelData per level (see allLevels() in Levels.h) and hand it to
// buildLevel() to spawn it into the scene.
struct LevelData
{
    QString     name;
    QStringList map;
};

#endif // LEVELDATA_H
