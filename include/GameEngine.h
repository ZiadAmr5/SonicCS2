#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "Goomba.h"

#include <QTimer>
#include <QElapsedTimer>
#include <QSet>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <player.h>
#include <QObject>
#include <QDebug>
#include <level.h>

class gameView;

// This class handles frames, ticks, and AABB push-out collisions
class gameLoop : public QObject
{
    Q_OBJECT
private:
    gameView* m_gv;
    QElapsedTimer TimeBetFrames; // Calculates deltatime
    QTimer frameRate;
    Player* m_p;
    level* m_l;

public:
    gameLoop(gameView* gv, Player* p, level* l);

public slots:
    void gameTick(); // Core loop managing the two-step (X then Y) physics resolution
};

class gameView : public QGraphicsView
{
    Q_OBJECT // Added macro if you ever need to use signals/slots inside gameView later
private:
    bool leftKeyPressed;
    bool rightKeyPressed;
    bool UpKeyPressed;
    bool DownKeyPressed;
    bool jumped = false;
    bool jumpHeld = false;
    double deltatime;
    double scaleFactor;


public:
    Goomba *goomba;
    Goomba *goomba2;
     QSet<int> pressedKeys;
    QGraphicsScene* scene; // Made public so gameLoop can easily read scene->collidingItems
    Player* mp;
    level* ml;

    gameView(Player* p = nullptr, level* l = nullptr, QWidget* parent = nullptr);

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void renderLevel(level& lev);

    // Scrolls the view to follow the player each frame (clamped to the world bounds).
    void updateCamera();

    void setDeltaTime(double dt) { deltatime = dt; }

    bool getrightKey()  { return rightKeyPressed; }
    bool getleftKey()   { return leftKeyPressed; }
    bool getJump()      { return jumped; }
    bool isJumpHeld()   { return jumpHeld; }

    void clearJump()    { jumped = false; }
};





#endif // GAMEENGINE_H
