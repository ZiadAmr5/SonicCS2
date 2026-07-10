#ifndef GAMEENGINE_H
#define GAMEENGINE_H



#include <QTimer>
#include <QElapsedTimer>
#include <QSet>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <player.h>
#include<QObject>
#include <QDebug>
class gameView;
//this class handles timers and updates functions every frame, implementation for this class is currently in gameview.cpp, should be moved to its own seperate cpp
class gameLoop:public QObject
{
    Q_OBJECT
private:
    gameView* m_gv;
    QElapsedTimer TimeBetFrames; // will be used to calculate deltatime
    QTimer frameRate;
    Player*m_p;
public:
    gameLoop(gameView* gv,Player*p);

public slots:

 void gameTick();



};

class gameView:public QGraphicsView
{
     QSet<int>pressedKeys; // set that stores key inputs, handles multiple presses
    bool leftKeyPressed;
    bool rightKeyPressed;
    bool UpKeyPressed;
    bool DownKeyPressed;
    bool jumped;
    double deltatime;
    double scaleFactor; // unused
    QGraphicsScene* scene;
    Player* mp;

public:
    gameView(Player*p = nullptr,QWidget* parent = nullptr);

     void keyPressEvent(QKeyEvent* event);
     void keyReleaseEvent(QKeyEvent* event);
     void setDeltaTime(double dt)

     {
         deltatime=dt;
     }
     bool getrightKey()
     {
         return rightKeyPressed;
     }
     bool getleftKey()
     {
         return leftKeyPressed;
     }
     bool getJump()
     {
         return jumped;
     }
     void updatePosition();


};

#endif // GAMEENGINE_H
