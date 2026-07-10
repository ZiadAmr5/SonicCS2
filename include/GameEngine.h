#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#endif // GAMEENGINE_H

#include <QTimer>
#include <QElapsedTimer>
#include <QSet>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <player.h>
class gameLoop
{



};

class gameView:public QGraphicsView
{
     QSet<int>pressedKeys;
    bool leftKeyPressed;
    bool rightKeyPressed;
    bool UpKeyPressed;
    bool DownKeyPressed;
    bool jumped;
    QGraphicsScene* scene;
    Player* mp;
public:
    gameView(Player*p = nullptr,QWidget* parent = nullptr);




     void keyPressEvent(QKeyEvent* event);
     void releaseKeyEvent(QKeyEvent* event);


};
