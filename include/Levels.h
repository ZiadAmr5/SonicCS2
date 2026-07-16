#ifndef LEVELS_H
#define LEVELS_H

#include <QVector>
#include "LevelData.h"

// =============================================================================
//  ====>  ALL LEVELS LIVE HERE  <====
//  Each LevelData below is one level. To add another, instantiate a new
//  LevelData, fill in its map, and add it to the returned list.
//
//  Symbols:  X solid | P start | E goal | C coin | N enemy (Goomba)
//            M Super Mushroom | F Fire Flower | (space) empty
//
//  DESIGN NOTE - what the player can actually reach with the current physics:
//    standing jump ~1.1 tiles | walking ~1.6 | running ~2.9 (max)
//  So never place a step/platform more than 2 tiles above the surface the
//  player jumps from, or it becomes impossible. Use staircases to go higher.
//  Ground occupies the bottom 3 rows; the player stands on row 11.
// =============================================================================
inline QVector<LevelData> allLevels()
{
    LevelData l1; l1.name = "Level 1"; l1.map = QStringList{
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                      CCCCCC                                                   E    ",
        "   P   CCCCCC            XX           XXXXXX                              CCCCCC         XX    E    ",
        "     T    B     M    N    XXXX      T           B  N    F  XXXXX          N            N    XXX    E    ",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
    };

    LevelData l2; l2.name = "Level 2"; l2.map = QStringList{
        "                                                                                                        ",
        "                                                                                                        ",
        "                                                                                                        ",
        "                                                                                                        ",
        "                                                                                                        ",
        "                                                                                                        ",
        "                                                                                                        ",
        "                                                                                                        ",
        "                                                                                                        ",
        "                  XX              CCCCCC                    CCCCCC                                  E   ",
        "  P     CCCCCC   XXXX            B XXXXXX    CCCCC      B     XXXXXX                           XX     E   ",
        "      M    N    XXXXXX              N       XXXXX         F    N      XXXXX           N     XXXX  N E   ",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXX"
    };

    LevelData l3; l3.name = "Level 3"; l3.map = QStringList{
        "                                                                                                                ",
        "                                                                                                                ",
        "                                                                                                                ",
        "                                                                                                                ",
        "                                                                                                                ",
        "                                                                                                                ",
        "                                                                                                                ",
        "                                                                                                                ",
        "                                                                                                                ",
        "              CCCCC      B                                                       CCCCC                 XX    E   ",
        " P   CCCCCC   XXXXX            XXX          B        CCCCC XXXXX                 XXXXX                XXX    E   ",
        "   M    N             B        XXXXX   N          FN XXXXX       N       N XXXXX               N     XXXX    E   ",
        "XXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXX"
    };

    return { l1, l2, l3 };
}

#endif // LEVELS_H
