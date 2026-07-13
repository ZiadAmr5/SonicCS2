
#include <player.h>
#include "Enemy.h"
#include "Coin.h"
#include <QDebug>
using namespace std;
//quick player rectangle for testing ( I think it is obvious where this came from, except velocityX and y)
Player::Player(QGraphicsItem *parent) : Entity(parent) ,groundSpeed(0)
{
    // Define a 40 width x 60 height rectangle placeholder
    setRect(0, 0, 40, 60);

    // Make it solid blue
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

    if (health < maxHealth) health++;      // gain health
    else                    addScore(100); // full health -> points instead

    if (coins >= 100) { coins -= 100; lives++; }  // 100 coins = +1 life

    qDebug() << "coin! coins:" << coins << "health:" << health << "score:" << score << "lives:" << lives;
}

void Player::powerUp(PowerState newState) { power = newState; /* TODO: grow/fire visuals */ }

// Lose one health point. Grants brief invincibility so a single contact costs one hit.
void Player::takeDamage(int amount)
{
    if (isInvincible) return;
    health -= amount;
    if (health < 0) health = 0;
    isInvincible     = true;
    invincibleFrames = 90; // ~1.5s at 60 FPS
    qDebug() << "hit! health now:" << health;
}

void Player::tickInvincibility()
{
    if (isInvincible && --invincibleFrames <= 0)
        isInvincible = false;
}









