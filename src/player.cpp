
#include <player.h>
#include "Enemy.h"
#include "Coin.h"
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include "Sfx.h"
using namespace std;

// Mario sprite sheet layout (assets/mario_frames.png, built from the SMM2/SMB3 sheet):
//   5 frames per row: 0 = idle, 1-3 = walk cycle, 4 = jump
//   row y-offsets:    Small y=0 h=16 | Big y=16 h=32 | Fire y=48 h=32
static const int  SPR_W     = 20;   // frame width in the sheet
static const double SPR_SCALE = 2.5; // 16px tall sprite -> 40px tile
//quick player rectangle for testing ( I think it is obvious where this came from, except velocityX and y)
Player::Player(QGraphicsItem *parent) : Entity(parent) ,groundSpeed(0)
{
    // Small Mario is one tile (40x40); the sprite is drawn over this hitbox in paint().
    setRect(0, 0, 40, 40);

    // Fallback colour if the sprite sheet is missing
    setBrush(QBrush(QColor(0, 100, 255)));
    setPen(QPen(Qt::NoPen)); // Removes the black outline border
    qDebug()<<"setting player in position.";
     this->setPos(400, 300);

     health = maxHealth; // start with full health (survive two hits)

}
//updates the physics , taken from the sonic physics guide on the website SonicRetro
 void Player:: physUpdate(bool right , bool left ,bool jumped,bool jumpHeld,bool runButtonPressed)
{// might include a case where both left and right are pressed , maybe a feature
     //qDebug()<<"calling the physics function";
     //qDebug()<<"Falling to the ground";
     tickInvincibility(); // count down i-frames each frame
     if (right)     facing = 1;   // remember which way we're facing (fireball direction)
     else if (left) facing = -1;
     // advance the walk cycle with distance travelled, so it speeds up when running
     animTime += deltatime * abs(velocityX) / 15.0;
     bool jumpingThisFrame = jumped && isOnGround;
     double slopeFactor =0.125;
     float currentMaxSpeed= runButtonPressed? maxSpeedRun:maxSpeedWalk;
     float currentAcceleration= isOnGround? acceleration:(acceleration*0.5f);
     float currentSkidding=isOnGround? skidDeceleration:(skidDeceleration*0.5f);

     if(right)
    {



         //going left then pressing right
         if(velocityX<0)
         {
             velocityX+=currentSkidding*deltatime; //scale per deltatime

         if(velocityX>0)
            {
             velocityX=0.5f;  //this makes it so that when the player turns, they would start with a speed = 0.5
            }
         }
         //going right
         else if(velocityX<currentMaxSpeed)
         {
             velocityX+=currentAcceleration*deltatime;
             //if (!isOnGround && velocityX >= currentSpeed)velocityX = currentSpeed; // mid air
             /*if(groundSpeed>=MaxSpeed) //this is a speed cap, should be removed later on when slopes are implemented
             {
                 groundSpeed=MaxSpeed;
             }
            */
             if(velocityX>currentMaxSpeed)
                 velocityX=currentMaxSpeed;
         }



     }
     else if (left)
     {//going right then pressing left
         if(velocityX>0)
         {
         velocityX-=currentSkidding*deltatime;
         if(velocityX<=0)
             {
           velocityX=-0.5f;
             }
         }
         //going left
         else if(velocityX>-currentMaxSpeed)
         {
            velocityX-=currentAcceleration*deltatime;
            // if (!isOnGround && groundSpeed <= -MaxSpeed) groundSpeed = -MaxSpeed;

             /*if(groundSpeed<=-MaxSpeed)
                 {
                 groundSpeed=-MaxSpeed;
                 }
            */
         }

     }

     else
         //apply friction, we subract from the smallest value  so we don't go past zero
     {
         if(isOnGround)

         {
         if(velocityX>0)
            {
             velocityX-=friction *deltatime;
             if(velocityX<0)
                 velocityX=0;
             }
         else if ( velocityX<0)
             {
             velocityX+=friction*deltatime;
             if(velocityX>0)
                 velocityX=0;
            }
         }

     }



     if(jumpingThisFrame)
     {
        // qDebug()<<"jump is true";
         //we subtract to preserve the speed before the jump, the functions are switched because we want to jump perpendicular to the surface we are on
        float baseJump =240.0f;
        float momentumBonus= abs(velocityX)*0.5f;
        //groundSpeed=velocityX;
        velocityY=-(baseJump+momentumBonus);
        isOnGround=false;
        isJumping=true;
        Sfx::play(Sfx::Jump);



     }
     if (!isOnGround)
     {
         qDebug()<<"Not on the ground";
         if(jumpHeld&&velocityY <0)
         {
             qDebug()<<"I AM HOLDING JUMP";

             velocityY+=jumpingGravity*deltatime;
         }


         else
         {
             qDebug()<<"ON THE GROUND";
         velocityY+=gravity*deltatime;
         }
         if(velocityY>maxTerminalVelocity)
         {
             velocityY=maxTerminalVelocity;
         }
     }





     qDebug() << "velocityX:" << velocityX <<"VelocityY"<<velocityY<<"Gravity:"<<gravity<< "MaxSpeed:" << currentMaxSpeed << "acceleration:" << acceleration << "Deceleration"<<currentSkidding;
     //qDebug()<<"A.hit:"<<A.hit<<"RayLength:"<<A.distance;
     //qDebug() << "Player pos:" << this->pos() << "leftFoot:" << leftFoot << "rightFoot:" << rightFoot;

}


// Which sprite frame to show: jump when airborne, walk cycle when moving, else idle.
int Player::currentFrame() const
{
    if (!isOnGround)            return 4;                       // jump
    if (abs(velocityX) > 5.0f)  return 1 + (int(animTime) % 3); // walk cycle
    return 0;                                                   // idle
}

void Player::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    static const QPixmap sheet(QStringLiteral(":/assets/assets/mario_frames.png"));

    if (sheet.isNull()) { // sprite sheet missing -> fall back to the plain coloured box
        painter->setBrush(brush());
        painter->setPen(pen());
        painter->drawRect(rect());
        return;
    }

    // pick the row for the current power state
    int srcY = 0, srcH = 16;
    if (power == PowerState::Big)  { srcY = 16; srcH = 32; }
    if (power == PowerState::Fire) { srcY = 48; srcH = 32; }
    const QRect src(currentFrame() * SPR_W, srcY, SPR_W, srcH);

    // Draw at a fixed scale (square pixels) anchored to the bottom-centre of the hitbox,
    // so the sprite lines up with the feet regardless of hitbox size.
    const QRectF box = rect();
    const double w = SPR_W * SPR_SCALE;
    const double h = srcH  * SPR_SCALE;
    const QRectF dst(box.center().x() - w / 2.0, box.bottom() - h, w, h);

    painter->save();
    if (facing < 0) { // mirror horizontally about the sprite's centre
        painter->translate(dst.center().x(), 0);
        painter->scale(-1, 1);
        painter->translate(-dst.center().x(), 0);
    }
    painter->drawPixmap(dst, sheet, src);
    painter->restore();
}

// --- UML gameplay behaviour (Mario) ---

void Player::addScore(int points) { score += points; }

// Killing an enemy (e.g. stomping a Goomba) increases the score.
void Player::stompEnemy(Enemy* e) { if (e) e->stomped(); addScore(200); }

// Collecting a coin:
//  - if not at full health, the coin heals +1 health
//  - if already at full health (2), the coin gives score instead
//  - every 100 coins grants an extra life
void Player::collectCoin(Coin* c)
{
    Q_UNUSED(c);
    coins++;
    Sfx::play(Sfx::Coin);

    if (health < maxHealth) health++;      // gain health
    else                    addScore(100); // full health -> points instead

    if (coins >= 100) { coins -= 100; lives++; Sfx::play(Sfx::OneUp); }  // 100 coins = +1 life

    qDebug() << "coin! coins:" << coins << "health:" << health << "score:" << score << "lives:" << lives;
}

// Apply a power-up state: Small (base), Big (mushroom) or Fire (fire flower).
// Big/Fire Mario is taller and can take one extra hit; Fire Mario can shoot fireballs.
void Player::powerUp(PowerState newState)
{
    const double oldH = rect().height();
    if (newState != power && newState != PowerState::Small)
        Sfx::play(Sfx::PowerUp); // only on an actual pick-up, not on reset
    power = newState;

    const double newH = (newState == PowerState::Small) ? 40.0 : 80.0; // 1 tile vs 2 tiles
    setRect(0, 0, 40, newH);
    // keep the feet planted when growing/shrinking
    setPos(pos().x(), pos().y() - (newH - oldH));

    maxHealth = (newState == PowerState::Small) ? 2 : 3;
    health    = maxHealth; // a power-up refills you

    switch (newState)
    {
    case PowerState::Small: setBrush(QBrush(QColor(0, 100, 255))); break; // blue
    case PowerState::Big:   setBrush(QBrush(QColor(220, 50, 50)));  break; // red
    case PowerState::Fire:  setBrush(QBrush(QColor(255, 140, 0)));  break; // orange
    }
    qDebug() << "power-up! state:" << int(newState) << "maxHealth:" << maxHealth;
}

// Lose one health point. Grants brief invincibility so a single contact costs one hit.
void Player::takeDamage(int amount)
{
    if (isInvincible) return;
    health -= amount;
    if (health < 0)
    {
        health = 0;
        setMaxHealth(health);
    }
    isInvincible     = true;
    invincibleFrames = 90; // ~1.5s at 60 FPS

    qDebug() << "hit! health now:" << health;

}

void Player::tickInvincibility()
{
    if (isInvincible && --invincibleFrames <= 0)
        isInvincible = false;
}









