#ifndef LEVELS_H
#define LEVELS_H

#include <QVector>
#include "LevelData.h"

// =============================================================================
//  ====>  ALL LEVELS LIVE HERE  <====
//  Each LevelData below is one level. To add another level, instantiate a new
//  LevelData, fill in its map, and add it to the returned list.
//  Symbols: X solid | P start | E goal | C coin | N enemy | (space) empty
// =============================================================================
inline QVector<LevelData> allLevels()
{
    LevelData l1; l1.name = "Level 1"; l1.map = QStringList{
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                              XXXXXXX                                         XXXXXXX               ",
        "                                                                                                    ",
        "            XXXXX                       XXXX              XXXXX                         XXXXX       ",
        "      xxxx          XXXX                                             XXXX                           ",
        "                     XXXXX                                              XXXXX                       ",
        "       xxxx                                                                                         ",
        "   P                          ccccc                                                             E   ",
        "XXXXXXXXX                    XXXXXXXX         CCCCCCC         XXXXXXXX                          E   ",
        "XXXXXXXXXXXXX  N          cccc        XXXXXXXX CCCCCCC     N                   XXXXXXXX         E   ",
        "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXX"
    };

    LevelData l2; l2.name = "Level 2"; l2.map = QStringList{
        "                                                                                          ",
        "                                                                                          ",
        "                                                                                          ",
        "                                                                      XXXXXXX             ",
        "                                                                                          ",
        "          XXXXXX                                  XXXXXX                                  ",
        "                         CCCCCC                                                           ",
        "                        XXXXXXX                                 XXXXXXX                   ",
        "                                        CCCCCCC                                           ",
        "   P                                    XXXXXXX                                        E  ",
        "                                                                                       E  ",
        "                  XXXXX          CCNCCCC                    N XXXXX        N           E  ",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
    };

    LevelData l3; l3.name = "Level 3"; l3.map = QStringList{
        "                                                                                                              ",
        "                                                                                                              ",
        "                                                                                                              ",
        "                                                                                                              ",
        "                              XXXXXXX                                                   XXXXXXX               ",
        "                                                                                                              ",
        "            XXXXXX                                     XXXXXX                                   CCCCCC        ",
        "                                                                                                              ",
        "                                        CCCCCC                        XXXXXX                                  ",
        "   P                                                                                                     E    ",
        "                         CCCCCC                                                      XXXXXX              E    ",
        "               N                        N         XXXXXXXXX CCCCCNCCCC                              N    E    ",
        "XXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
    };

    return { l1, l2, l3 };
}

#endif // LEVELS_H
