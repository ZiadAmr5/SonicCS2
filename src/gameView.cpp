#include "GameEngine.h"
#include "LevelMap.h"
#include "goomba.h"
gameView::gameView(Player *p, level *l, QWidget *parent)
    : scene(nullptr)
    , mp(p)
    , QGraphicsView(parent)
    , leftKeyPressed(false)
    , rightKeyPressed(false)
    , DownKeyPressed(false)
    , UpKeyPressed(false)
{
    scene = new QGraphicsScene(this);
    setFocusPolicy(Qt::StrongFocus);

    // --- Camera setup ---
    // Hide the scrollbars; the camera drives the view via updateCamera() instead of the user scrolling.
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ml = l;

    scene->addItem(mp); // Add player first so the scene tracks them
   goomba = new Goomba();
    goomba->setPos(700,440);
    scene->addItem(goomba);
    goomba2 = new Goomba();
    goomba2->setPos(400, 400);
    scene->addItem(goomba2);


    // Build the editable level (edit include/LevelMap.h). This places the ground/platform
    // blocks, positions the player, and sizes the world for the camera to scroll within.
    buildLevel(scene, mp);

    // Load collisionBoundaries straight from the blocks we just placed in the scene.
    ml->loadFromUiScene(scene);

        // Render visual boundaries only if you want to see them for debugging!
    /*
    renderLevel(*ml);
*/

    scaleFactor = scene->width() / 320.0;
    mp->getScaleFactor(scaleFactor);
    setScene(scene);

    updateCamera(); // frame the player on the very first frame
}

void gameView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right) {
        // rightKeyPressed = true;
        pressedKeys.insert(event->key());
    } else if (event->key() == Qt::Key_Left) {
        // leftKeyPressed = true;
        pressedKeys.insert(event->key());
    } else if (event->key() == Qt::Key_Space) {
        jumped = true;
        //jumpHeld = true;
        pressedKeys.insert(event->key());
    } else if (event->key() == Qt::Key_Z) {
        pressedKeys.insert(event->key());
    }
}

void gameView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right) {
        rightKeyPressed = false;
        pressedKeys.remove(event->key());
    } else if (event->key() == Qt::Key_Left) {
        leftKeyPressed = false;
        pressedKeys.remove(event->key());
    } else if (event->key() == Qt::Key_Space) {
        jumpHeld = false;
    } else if (event->key() == Qt::Key_Z) {
        pressedKeys.remove(event->key());
    }
}
/*
void gameView::updatePosition()
{
    float vx = mp->getHorizontalSpeed();
    float vy = mp->getVerticalSpeed();
    mp->setPos(mp->pos().x() + vx * deltatime, mp->pos().y() + vy * deltatime);
}
*/

void gameView::renderLevel(level &lev)
{
    // If you aren't rendering static background images anymore, this just outlines your hitboxes
    for (const collisionBoundary &b : lev.getcollisionBoundaries()) {
        auto *item = new QGraphicsRectItem(0, 0, b.w, b.l);
        item->setPos(b.x, b.y);
        item->setPen(QPen(Qt::red, 2)); // Draw red outlines around your blocks for debugging
        scene->addItem(item);
    }
}

void gameView::updateCamera()
{
    if (!mp)
        return;

    // Follow the player horizontally; look a little ahead in the direction of travel so the
    // player sees what's coming. Vertical stays locked to the world's mid-line (single-screen-tall level).
    qreal playerX = mp->sceneBoundingRect().center().x();
    qreal lookahead = qBound(-140.0, mp->getHorizontalSpeed() * 0.35, 140.0);
    qreal camX = playerX + lookahead;
    qreal camY = sceneRect().center().y();

    // centerOn() clamps to sceneRect(), so the camera never scrolls past the world edges.
    centerOn(camX, camY);
}

gameLoop::gameLoop(gameView *gv, Player *p, level *l)
    : m_gv(gv)
    , m_p(p)
    , m_l(l)
{
    frameRate.start(16);
    TimeBetFrames.start();
    connect(&frameRate, &QTimer::timeout, this, &gameLoop::gameTick);
}

void gameLoop::gameTick()
{

    double deltatime = TimeBetFrames.restart() / 1000.0;
    m_p->setDeltaTime(deltatime);
    m_gv->setDeltaTime(deltatime);
    bool right = m_gv->pressedKeys.contains(Qt::Key_Right);
    bool left = m_gv->pressedKeys.contains(Qt::Key_Left);
    bool jumpheld = m_gv->pressedKeys.contains(Qt::Key_Space);
    bool run = m_gv->pressedKeys.contains(Qt::Key_Z);
    // 1. Calculate speeds based on inputs (No sensors passed!)
    m_p->physUpdate(right, left, m_gv->getJump(), jumpheld, run);
    if(m_gv->goomba)
    {
        m_gv->goomba->update();
    }
    if (m_gv->goomba2)
    {
        m_gv->goomba2->update();
    }
    m_gv->clearJump();

    double vx = m_p->getHorizontalSpeed();
    double vy = m_p->getVerticalSpeed();
    double oldY = m_p->y();
    //the entirety of the collision system is here, Axis-Aligned Boundary Box (AXBB) we check for overlaps between the player and blocks then push the player

    m_p->setPos(m_p->pos().x() + (vx * deltatime), m_p->pos().y()); // teleport mario to the front

    for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p)) // is mario colliding with anything?
    {

        if (m_p->collidesWithItem(item)) {

            QRectF blockRect = item->sceneBoundingRect(); // boundaries of the block

            QRectF playerRect = m_p->sceneBoundingRect(); //boundaries of the player

            if (vx > 0) {
                m_p->setPos(blockRect.left() - playerRect.width(), m_p->pos().y());
                m_p->setHorizontalSpeed(0);
            } else if (vx < 0) {
                m_p->setPos(blockRect.right(), m_p->pos().y());
                m_p->setHorizontalSpeed(0);
            }
        }
    }

    m_p->setPos(m_p->pos().x(), m_p->pos().y() + (vy * deltatime));

    // Assume airborne each frame; only a landing (below) re-confirms grounded.
    // Without this, walking off a ledge leaves isOnGround stuck true and gravity never re-applies.
    m_p->setIsOnGround(false);

    for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p)) {
        if (m_p->collidesWithItem(item)) {
            if (item == m_gv->goomba)
            {
                QRectF enemy = m_gv->goomba->sceneBoundingRect();

                double oldBottom = oldY + m_p->boundingRect().height();

                bool stomp =
                    vy > 0 &&
                    oldBottom <= enemy.top() + 2;
                if (stomp)
                {
                    m_gv->scene->removeItem(m_gv->goomba);
                    delete m_gv->goomba;
                    m_gv->goomba = nullptr;

                    m_p->setVerticalSpeed(-180);

                    return;
                }

                qDebug() << "PLAYER HIT";
                m_gv->scene->removeItem(m_p);
                return;
            }

            if (item == m_gv->goomba2)
            {
                QRectF enemy = m_gv->goomba->sceneBoundingRect();

                double oldBottom = oldY + m_p->boundingRect().height();
                bool stomp =
                    vy > 0 &&
                    oldBottom <= enemy.top() + 2;
                if (stomp)
                {
                    m_gv->scene->removeItem(m_gv->goomba2);
                    delete m_gv->goomba2;
                    m_gv->goomba2 = nullptr;

                    m_p->setVerticalSpeed(-180);

                    return;
                }

                qDebug() << "PLAYER HIT";
                m_gv->scene->removeItem(m_p);
                return;
            }
            QRectF blockRect = item->sceneBoundingRect();

            QRectF PlayerRect = m_p->sceneBoundingRect();

            if (vy < 0) {
                m_p->setPos(m_p->pos().x(), blockRect.bottom());
                m_p->setVerticalSpeed(0);
                m_p->setIsOnGround(false);
            }

            else if (vy > 0) {
                m_p->setPos(m_p->pos().x(), blockRect.top() - PlayerRect.height());
                m_p->setIsOnGround(true);
                m_p->setVerticalSpeed(0);
            }
        }
    }

    // 3. Camera follows the player after its position is fully resolved this frame.
    m_gv->updateCamera();
    // vertical collision loop ends here



    /*if (item == m_p || item->data(0).toString() != "solid") continue; //if not continue

        QRectF blockRect = item->sceneBoundingRect(); // boundaries of the block
        QRectF playerRect = m_p->sceneBoundingRect(); //boundaries of the player

        if (vx > 0) {       // Ran into a right wall -> push back left
            m_p->setPos(blockRect.left() - playerRect.width(), m_p->pos().y());
        } else if (vx < 0) { // Ran into a left wall -> push back right
            m_p->setPos(blockRect.right(), m_p->pos().y());
        }

        m_p->setHorizontalSpeed(0);
        break; // Core resolution done, skip evaluating subsequent items this tick
    }

    //same thing but for y axis
    m_p->setPos(m_p->pos().x(), m_p->pos().y() + (vy * deltatime));
    m_p->setIsOnGround(false); // Default to airborne until floor overlap confirmed

    for (QGraphicsItem* item : m_gv->scene->collidingItems(m_p))
    {
        if (item == m_p || item->data(0).toString() != "solid") continue;

        QRectF blockRect = item->sceneBoundingRect();
        QRectF playerRect = m_p->sceneBoundingRect();

        if (vy > 0) {       // falling and touching a block down
            m_p->setPos(m_p->pos().x(), blockRect.top() - playerRect.height());
            m_p->setVerticalSpeed(0);
            m_p->setIsOnGround(true);
        }
        else if (vy < 0) { //jumping and hitting a block
            m_p->setPos(m_p->pos().x(), blockRect.bottom());
            m_p->setVerticalSpeed(0);
        }
        break;
    }
    */

    }
