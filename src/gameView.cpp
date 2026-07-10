#include "GameEngine.h"

gameView::gameView(Player* p ,QWidget* parent ):scene(nullptr), mp(p),QGraphicsView(parent),leftKeyPressed(false),rightKeyPressed(false),DownKeyPressed(false),UpKeyPressed(false)
{
    scene=new QGraphicsScene(this);
    scene->setSceneRect(0,0,800,600);
    setFocusPolicy(Qt::StrongFocus);

}
void gameView::keyPressEvent(QKeyEvent* event)
{
    if(event->key()==Qt::Key_Right)
    {
        rightKeyPressed=true;
        pressedKeys.insert(event->key());
        mp->physUpdate(rightKeyPressed,leftKeyPressed,jumped);
    }
    else if(event->key()==Qt::Key_Left)
    {
        leftKeyPressed=true;
        pressedKeys.insert(event->key());
        mp->physUpdate(rightKeyPressed,leftKeyPressed,jumped);
    }
}
