
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

}
//updates the physics , taken from the sonic physics guide on the website SonicRetro
 void Player:: physUpdate(bool right , bool left ,bool jumped,bool jumpHeld,bool runButtonPressed)
{// might include a case where both left and right are pressed , maybe a feature
     //qDebug()<<"calling the physics function";
     //qDebug()<<"Falling to the ground";
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
        float momentumBonus= abs(velocityX)*0.4f;
        //groundSpeed=velocityX;
        velocityY=-(baseJump+momentumBonus);
        isOnGround=false;
        isJumping=true;



     }
     else if(isJumping &&!jumpHeld&& velocityY<0)g
     {
         velocityY*=0.5;
         isJumping=false;
     }
     if (!isOnGround)
     {
         if(jumpHeld&&velocityY <0)
              velocityY+=jumpingGravity*deltatime;
         else
         {
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


// --- UML gameplay behaviour (Mario) — stubs; to be wired into the game loop ---
void Player::stompEnemy(Enemy* e)          { Q_UNUSED(e); /* TODO: bounce off enemy, e->stomped() */ }
void Player::collectCoin(Coin* c)          { Q_UNUSED(c); coins++; addScore(100); /* TODO: sfx/HUD */ }
void Player::powerUp(PowerState newState)  { power = newState; /* TODO: grow/fire visuals */ }
void Player::addScore(int points)          { score += points; }









