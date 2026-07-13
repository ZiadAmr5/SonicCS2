#ifndef GAMEENGINE_H
#define GAMEENGINE_H

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
class QLabel;

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
    bool m_finished = false; // set once the game ends (win or lose)

public:
    gameLoop(gameView* gv, Player* p, level* l);

    void finishLevel(); // stop the loop and show the "level finished" message
    void gameOver();    // stop the loop and show the "game over" message

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

    QLabel* hud = nullptr;  // on-screen score / coins / lives / health display
    void updateHud();       // refresh the HUD text from the player

    void setDeltaTime(double dt) { deltatime = dt; }

    bool getrightKey()  { return rightKeyPressed; }
    bool getleftKey()   { return leftKeyPressed; }
    bool getJump()      { return jumped; }
    bool isJumpHeld()   { return jumpHeld; }

    void clearJump()    { jumped = false; }
};

#endif // GAMEENGINE_H
