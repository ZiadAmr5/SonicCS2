#include <QAudioOutput>
#include <QLabel>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QPainter>
#include <QSet>
#include "Coin.h"
#include "Endpoint.h"
#include "Fireball.h"
#include "GameEngine.h"
#include "LevelMap.h"
#include "Levels.h"
#include "PowerUp.h"
#include "Sfx.h"
#include "goomba.h"
#include <cmath>
#include "bbill.h"
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

    setScene(scene);
    scene->addItem(mp); // add the player once; loadLevel() keeps it across level swaps

    // --- HUD overlay (floats over the view, doesn't scroll with the camera) ---
    hud = new QLabel(this);
    hud->setStyleSheet("color: white; background: rgba(0,0,0,140);"
                       "padding: 4px 8px; font: bold 15px 'Consolas';");
    hud->move(10, 10);
    hud->raise();

    // --- Audio: preload the effects and start the looping overworld theme ---
    Sfx::preload();
    {
        auto *music = new QMediaPlayer(this);
        auto *out = new QAudioOutput(this);
        music->setAudioOutput(out);
        music->setSource(QUrl(QStringLiteral("qrc:/assets/assets/smb3_overworld.wav")));
        music->setLoops(QMediaPlayer::Infinite);
        out->setVolume(0.35); // sit under the sound effects
        music->play();
    }

    // Load all levels (edit them in Levels.h) and build the first one.
    levels = allLevels();
    loadLevel(0);
}

// Clears the current level from the scene and builds the given level in its place.
void gameView::loadLevel(int index)
{
    if (index < 0 || index >= levels.size())
        return;
    currentLevel = index;

    // Rebuild the scene without destroying the player object.
    if (mp->scene() == scene)
        scene->removeItem(mp);
    scene->clear(); // deletes the old level's blocks/coins/enemies/outlines
    scene->addItem(mp);

    // Reset the player's motion; its position is set by 'P' inside buildLevel().
    mp->setHorizontalSpeed(0);
    mp->setVerticalSpeed(0);
    mp->setGroundSpeed(0);
    mp->setIsOnGround(false);

    buildLevel(scene, mp, levels[index]);
    ml->loadFromUiScene(scene); // collision boundaries from the solid blocks
    // renderLevel(*ml);          // (debug) red hitbox outlines - re-enable to see collision boxes

    scaleFactor = scene->width() / 320.0;
    mp->getScaleFactor(scaleFactor);

    updateCamera(); // frame the player on the first frame of the level
    updateHud();
}

void gameView::loadNextLevel()
{
    if (hasNextLevel())
        loadLevel(currentLevel + 1);
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
        qDebug() << "Pressing jump";
        pressedKeys.insert(event->key());
    } else if (event->key() == Qt::Key_Z) {
        pressedKeys.insert(event->key());
    } else if (event->key() == Qt::Key_X) {
        if (!event->isAutoRepeat())
            shoot = true; // Fire Mario throws a fireball
    }
    else if (event->key() == Qt::Key_Down)
    {
        pressedKeys.insert(event->key());
    }
}

void gameView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_Right) {
        rightKeyPressed = false;
        pressedKeys.remove(event->key());
    } else if (event->key() == Qt::Key_Left) {
        leftKeyPressed = false;
        pressedKeys.remove(event->key());
    } else if (event->key() == Qt::Key_Space) {
        jumpHeld = false;
        pressedKeys.remove(event->key());
        QGraphicsView::keyReleaseEvent(event);
    } else if (event->key() == Qt::Key_Z) {
        pressedKeys.remove(event->key());
    }
    else if (event->key() == Qt::Key_Down)
    {
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

// NES-style backdrop: fill the sky, then tile the scenery strip horizontally across the world.
// The strip is 600px tall with its bushes sitting on y=480 - the same ground line as our levels.
void gameView::drawBackground(QPainter *painter, const QRectF &rect)
{
    static const QPixmap backdrop(QStringLiteral(":/assets/assets/backdrop.png"));
    const QColor sky(156, 252, 240); // SMB3 overworld sky

    painter->fillRect(rect, sky);
    if (backdrop.isNull())
        return; // no asset -> plain sky

    const int w = backdrop.width();
    const int x0 = int(std::floor(rect.left() / w)) * w; // snap to the tile grid
    for (int x = x0; x < int(rect.right()) + w; x += w)
        painter->drawPixmap(x, 0, backdrop);
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

void gameView::updateHud()
{
    if (!hud || !mp)
        return;

    QString powerName = QStringLiteral("Small");
    if (mp->getPower() == PowerState::Big)
        powerName = QStringLiteral("Big");
    if (mp->getPower() == PowerState::Fire)
        powerName = QStringLiteral("Fire");

    hud->setText(
        QStringLiteral("Score: %1    Coins: %2/100    Lives: %3    Health: %4/%5    Power: %6")
            .arg(mp->getScore())
            .arg(mp->getCoins())
            .arg(mp->getLives())
            .arg(mp->getHealth())
            .arg(mp->getMaxHealth())
            .arg(powerName));
    hud->adjustSize();
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

void gameLoop::finishLevel()
{
    Sfx::play(Sfx::LevelClear);

    // More levels to play? Advance and keep going.
    if (m_gv->hasNextLevel()) {
        m_gv->loadNextLevel();
        return;
    }

    // Last level finished -> win.
    m_finished = true;
    frameRate.stop(); // freeze the game
    QMessageBox::information(m_gv,
                             QStringLiteral("You Win!"),
                             QStringLiteral("\xF0\x9F\x8F\x81  All levels complete!"));
}

void gameLoop::gameOver()
{
    m_finished = true;
    frameRate.stop(); // freeze the game
    QMessageBox::information(m_gv,
                             QStringLiteral("Game Over"),
                             QStringLiteral("\xF0\x9F\x92\x80  Game Over"));
}

// Player died: spend a life and restart the level, or game over when out of lives.
void gameLoop::die()
{
    Sfx::play(Sfx::Die);
    m_p->loseLife();
    if (m_p->getLives() > 0) {
        m_p->resetPower(); // dying drops you back to Small Mario
        m_p->resetHealth();
        m_gv->loadLevel(m_gv->currentLevel); // retry the current level
    } else {
        gameOver();
    }
}

// Jump straight to a level (Levels menu). Resets health and resumes the loop if stopped.
void gameLoop::goToLevel(int index)
{
    m_finished = false;
    m_p->resetPower();
    m_p->resetHealth();
    m_gv->loadLevel(index);
    if (!frameRate.isActive()) {
        TimeBetFrames.restart();
        frameRate.start(16);
    }
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

    m_p->physUpdate(right, left, m_gv->getJump(), jumpheld, run);
    m_gv->clearJump(); // Clear jump trigger flag for next frame

    // Patrol every Goomba in the scene.
    for (QGraphicsItem *it : m_gv->scene->items())
        if (Goomba *g = dynamic_cast<Goomba *>(it))
            g->update();
    for (QGraphicsItem *item : m_gv->scene->items())
    {
        BBill *b = dynamic_cast<BBill *>(item);

        if (b)
            b->update();
    }

    // --- Fire Mario: throw a fireball on X ---
    if (m_gv->getShoot()) {
        m_gv->clearShoot();
        if (m_p->canShoot()) {
            const int dir = m_p->getFacing();
            auto *fb = new Fireball(dir * 420.0);
            fb->setRect(0, 0, 14, 14);
            const QRectF pr = m_p->sceneBoundingRect();
            fb->setPos(dir > 0 ? pr.right() + 2 : pr.left() - 16, pr.center().y() - 7);
            fb->setBrush(QBrush(QColor(255, 80, 0)));
            fb->setPen(QPen(QColor(120, 30, 0), 1));
            fb->setData(0, QStringLiteral("fireball"));
            m_gv->scene->addItem(fb);
            Sfx::play(Sfx::Fireball);
        }
    }

    // --- Fireballs: move, kill Goombas, despawn on walls / when spent ---
    {
        QSet<QGraphicsItem *> dead;
        for (QGraphicsItem *it : m_gv->scene->items()) {
            Fireball *fb = dynamic_cast<Fireball *>(it);
            if (!fb)
                continue;

            fb->step(deltatime);
            if (fb->expired()) {
                dead.insert(fb);
                continue;
            }

            for (QGraphicsItem *hit : m_gv->scene->collidingItems(fb)) {
                if (Goomba *g = dynamic_cast<Goomba *>(hit)) {
                    dead.insert(g);
                    dead.insert(fb);
                    m_p->addScore(200); // fireball kill scores like a stomp
                    break;
                }
                if (hit->data(0).toString() == "solid") {
                    dead.insert(fb);
                    break;
                }
            }
        }
        // delete after iterating so we never touch a freed pointer
        for (QGraphicsItem *it : dead) {
            m_gv->scene->removeItem(it);
            delete it;
        }
    }

    double vx = m_p->getHorizontalSpeed();
    double vy = m_p->getVerticalSpeed();
    double oldBottom = m_p->sceneBoundingRect().bottom(); // player's feet before moving (for stomp)

    //the entirety of the collision system is here, Axis-Aligned Boundary Box (AXBB) we check for overlaps between the player and blocks then push the player

    m_p->setPos(m_p->pos().x() + (vx * deltatime), m_p->pos().y()); // teleport mario to the front

    for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p)) // is mario colliding with anything?
    {
        if (m_p->collidesWithItem(item)) {
            if (item->data(0).toString() != "solid")
                continue; // only solid blocks are walls (skip goal + debug outlines)

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
    bool groundFound = false; //will be used to control the isOnGround
    QRectF PlayerRect = m_p->sceneBoundingRect();

    for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p)) //compare
    {
        if (m_p->collidesWithItem(item)) {
            if (item->data(0).toString() != "solid")
                continue; // only solid blocks stop the player (skip goal + outlines)
            //later on we should add other types by adding other conditions (coins,enemies,etc)

            QRectF blockRect = item->sceneBoundingRect();

            if (vy < 0) // hit a block above you
            {
                m_p->setPos(m_p->pos().x(), blockRect.bottom());
                m_p->setVerticalSpeed(0);

            }

            else if (vy > 0) //hit a block below you
            {
                m_p->setPos(m_p->pos().x(), blockRect.top() - PlayerRect.height());
                groundFound = true;
                m_p->setVerticalSpeed(0);
            }
        }
    }
    if (!groundFound) {
        QRectF groundSensor(PlayerRect.x(),
                            PlayerRect.bottom(),
                            PlayerRect.width(),
                            1.0f); //sensor below the player's feet
        QList<QGraphicsItem *> itemsUnder = m_gv->scene->items(
            groundSensor); // store everything that this sensor touches into this list
        for (QGraphicsItem *item : itemsUnder)
            if (item->data(0).toString()
                == "solid") //only solid ground counts (skip goal + outlines)
            {
                groundFound = true;
                m_p->setVerticalSpeed(0);
                break;
            }
    }
    m_p->setIsOnGround(groundFound);

    // --- Pit death: falling below the world costs a life (retry or game over) ---
    if (m_p->sceneBoundingRect().top() > m_gv->scene->sceneRect().bottom() + 80) {
        die();
        return;
    }

    // --- Enemies: a stomp from above kills the Goomba (+score); side contact hurts the player ---
    for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p)) {
        Goomba *g = dynamic_cast<Goomba *>(item);
        if (!g)
            continue;

        double enemyTop = g->sceneBoundingRect().top();
        bool stomped = (vy > 0) && (oldBottom <= enemyTop + 8); // came down onto its head
        if (stomped) {
            m_gv->scene->removeItem(g);
            delete g;
            m_p->addScore(200);          // killing an enemy increases the score
            m_p->setVerticalSpeed(-250); // bounce off the top
            Sfx::play(Sfx::Stomp);
        } else {
            m_p->takeDamage(1); // side contact: lose one health point
            if (m_p->isDead()) {
                die();
                return;
            }
        }
        break; // resolve one enemy interaction per frame
    }
    for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p))
    {
        BBill *b = dynamic_cast<BBill *>(item);

        if (!b)
            continue;

        double enemyTop = b->sceneBoundingRect().top();

        bool stomped = (vy > 0) && (oldBottom <= enemyTop + 8);

        if (stomped)
        {
            m_gv->scene->removeItem(b);
            delete b;

            m_p->addScore(200);

            // Bounce Mario upward
            m_p->setVerticalSpeed(-250);
        }
        else
        {
            m_p->takeDamage(1);

            if (m_p->isDead())
            {
                die();
                return;
            }
        }

        break;
    }
    // --- Pipes ---
    for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p))
    {
        if (item->data(0).toString() != "pipe")
            continue;

        if (m_gv->pressedKeys.contains(Qt::Key_Down))
        {
            m_p->setPos(1450, 420);
            m_p->setHorizontalSpeed(0);
            m_p->setVerticalSpeed(0);
            break;
        }
    }
    // --- Coins: collect on contact (heal / score / extra-life logic lives in Player) ---
    for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p)) {
        if (Coin *coin = dynamic_cast<Coin *>(item)) {
            m_p->collectCoin(coin);
            m_gv->scene->removeItem(coin);
            delete coin;
        }
    }

    // --- Power-ups: Super Mushroom / Fire Flower on contact ---
    for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p)) {
        if (PowerUp *pu = dynamic_cast<PowerUp *>(item)) {
            pu->apply(m_p); // Mushroom -> Big, Fire Flower -> Fire
            m_gv->scene->removeItem(pu);
            delete pu;
            break;
        }
    }

    //  Camera follows the player after its position is fully resolved this frame.
    m_gv->updateCamera();

    // Level-finish: if the player reaches the Endpoint goal, end the level.
    if (!m_finished) {
        for (QGraphicsItem *item : m_gv->scene->collidingItems(m_p)) {
            if (Endpoint *ep = dynamic_cast<Endpoint *>(item)) {
                if (ep->isReachedBy(m_p)) {
                    finishLevel();
                    break;
                }
            }
        }
    }

    m_gv->updateHud(); // refresh score / coins / lives / health display

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
