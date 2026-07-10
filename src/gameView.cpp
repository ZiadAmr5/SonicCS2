#include "GameEngine.h"

gameView::gameView(Player* p ,QWidget* parent ):scene(nullptr), mp(p),QGraphicsView(parent),leftKeyPressed(false),rightKeyPressed(false),DownKeyPressed(false),UpKeyPressed(false)
{
    scene=new QGraphicsScene(this);
    scene->setSceneRect(0,0,800,600);
    setFocusPolicy(Qt::StrongFocus);
    scene-> addItem(mp);
    scaleFactor = scene->width()/320.0; //scale factor is unsed for now
    mp->getScaleFactor(scaleFactor);
    setScene(scene);


}
void gameView::keyPressEvent(QKeyEvent* event)
{
    if(event->key()==Qt::Key_Right)
    {
        rightKeyPressed=true;
        pressedKeys.insert(event->key());
    }
    else if(event->key()==Qt::Key_Left)
    {
        leftKeyPressed=true;
        pressedKeys.insert(event->key());
    }

}
void gameView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Right)
    {

        rightKeyPressed=false;
        pressedKeys.remove((event->key()));


    }
    else if(event->key()==Qt::Key_Left)
    {
        leftKeyPressed=false;
        pressedKeys.remove(event->key());

    }
}
void gameView::updatePosition()
{
    float vx= mp->getHorizontalSpeed();
    float vy=mp->getVerticalSpeed();
    mp->setPos(mp->pos().x()+vx*deltatime,mp->pos().y()+vy*deltatime);
}

gameLoop::gameLoop(gameView* gv,Player* p):m_gv(gv),m_p(p)
{
    frameRate.start(16); // 1frame = 16 ms to achieve 60 fps
    TimeBetFrames.start();
    connect(&frameRate,&QTimer::timeout,this,&gameLoop::gameTick);
}


void gameLoop::gameTick()
{
    double deltatime= TimeBetFrames.restart()/1000.0;
    m_p->setDeltaTime(deltatime);
    m_gv->setDeltaTime(deltatime);
    m_p->physUpdate(m_gv->getrightKey(),m_gv->getleftKey(),m_gv->getJump());
    m_gv->updatePosition();

}
