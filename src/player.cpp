
#include <player.h>
#include <QDebug>
using namespace std;
//quick player rectangle for testing ( I think it is obvious where this came from, except velocityX and y)
Player::Player(QGraphicsItem *parent) : QGraphicsRectItem(parent) ,groundSpeed(0)
{
    // Define a 40 width x 60 height rectangle placeholder
    setRect(0, 0, 40, 60);

    // Make it solid blue
    setBrush(QBrush(QColor(0, 100, 255)));
    setPen(QPen(Qt::NoPen)); // Removes the black outline border
    qDebug()<<"setting player in position.";
     this->setPos(400, 300);
   widthsensorRadius= 40/2.0;
     lengthsensorRadius =60/2.0;

}
//updates the physics , taken from the sonic physics guide on the website SonicRetro
 void Player:: physUpdate(bool right , bool left ,bool jumped,bool jumpHeld,RayHit A,RayHit B)
{// might include a case where both left and right are pressed , maybe a feature
     //qDebug()<<"calling the physics function";
     //qDebug()<<"Falling to the ground";
     bool jumpingThisFrame = jumped && isOnGround;
     double slopeFactor =0.125;

     if(right)
    {


         //going left then pressing right
         if(groundSpeed<0)
         {
             groundSpeed+=deceleration*deltatime; //scale per deltatime

         if(groundSpeed>=0)
            {
             groundSpeed=0.5;  //this makes it so that when the player turns, they would start with a speed = 0.5
            }
         }
         //going right
         else if(groundSpeed<MaxSpeed)
         {
             groundSpeed+=acceleration*deltatime;
             if (!isOnGround && groundSpeed >= MaxSpeed) groundSpeed = MaxSpeed; // mid air
             /*if(groundSpeed>=MaxSpeed) //this is a speed cap, should be removed later on when slopes are implemented
             {
                 groundSpeed=MaxSpeed;
             }
            */
         }



     }
     else if (left)
     {//going right then pressing left
         if(groundSpeed>0)
         {
         groundSpeed-=deceleration*deltatime;
         if(groundSpeed<=0)
             {
             groundSpeed=-0.5;
             }
         }
         //going left
         else if(groundSpeed>-MaxSpeed)
         {
             groundSpeed-=acceleration*deltatime;
             if (!isOnGround && groundSpeed <= -MaxSpeed) groundSpeed = -MaxSpeed;

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

         groundSpeed-=min(abs(groundSpeed),friction)*deltatime*sgn(velocityX);

     }
     if((A.hit || B.hit) && velocityY >= 0)
     {
         //qDebug()<<"Hit Confirmed";
         RayHit ground =(A.hit&&(!B.hit||A.distance<B.distance))? A:B;// gets the shortest ray from the two

         this->groundAngle = qAtan2(ground.edge.dy(),ground.edge.dx());
         double Angle= this->groundAngle*180.0/M_PI;
         //groundSpeed+=slopeFactor;
         //velocityY= groundSpeed* qSin(this->groundAngle);
         //velocityX=groundSpeed*qCos(this->groundAngle);
         collisionMode currentMode= getCollisionMode(Angle);


         bool isSlippingOrFalling = false;
         if (currentMode == leftWall || currentMode == rightWall) {
             if (qAbs(groundSpeed) < 2.5) isSlippingOrFalling = true;
         } else if (currentMode == ceiling) {
             if (qAbs(groundSpeed) < 2.5) {
                 isSlippingOrFalling = true;
                 groundSpeed = 0; // Drop straight down safely
             }
         }

         if (isSlippingOrFalling)
         {
             // Detach from the surface and drop straight into the gravity pipeline
             isOnGround = false;
             groundAngle = 0.0;
             velocityY += gravity * deltatime;
             if (velocityY > MaxSpeed)        velocityY = MaxSpeed;
             else if (velocityY < -MaxSpeed) velocityY = -MaxSpeed;
             velocityX = groundSpeed;
         }
         else
         {
             isOnGround = true;

             // Classic SPG Slope Factor Resistance
             groundSpeed += slopeFactor * qSin(this->groundAngle);
             // Clean vector projection tracking raw radians
             velocityX = groundSpeed * qCos(this->groundAngle);
             velocityY = groundSpeed * qSin(this->groundAngle);


         }

         double playerWidth=40;
         double playerLength=60;
         switch(currentMode)
         {
         case (floor):
             this->setPos(this->pos().x(), ground.point.y() - playerLength);
             break;
         case (leftWall):
             this->setPos(ground.point.x(), this->pos().y());
             break;
         case (rightWall):
             this->setPos(ground.point.x() - playerWidth, this->pos().y());
             break;
         case (ceiling):
             this->setPos(this->pos().x(), ground.point.y());
             break;
         }







         /*isOnGround=true;
         groundAngle = qAtan2(ground.edge.dy(),ground.edge.dx());
         groundSpeed -= slopeFactor * qSin(groundAngle);
         velocityX = groundSpeed *qCos(groundAngle);
         velocityY=groundSpeed *qSin(groundAngle); // add negative if behavior changes
         if (abs(groundAngle) < 0.001) {
             velocityY = 0;
         }
         this->setPos(this->pos().x(),ground.point.y()-60);
        */

     }
     else
     {
         isOnGround = false;
         if (qAbs(groundSpeed) < 2.5) {
             groundAngle = 0.0;
         }
         velocityY+=gravity*deltatime;
         if(velocityY>MaxSpeed)
             velocityY=MaxSpeed;
         else if (velocityY<-MaxSpeed)
             velocityY=-MaxSpeed;

         velocityX=groundSpeed;

     }


     if(jumpingThisFrame)
     {
        // qDebug()<<"jump is true";
         //we subtract to preserve the speed before the jump, the functions are switched because we want to jump perpendicular to the surface we are on
         double launchAngle= this->groundAngle-(M_PI/2.0);
         velocityX-=jumpForce*qSin(launchAngle);
        velocityY-=jumpForce*qCos(launchAngle);
         isJumping=true;
         isOnGround=false;
     }
     else if(isJumping &&!jumpHeld&& velocityY<0)
     {
         velocityY*=0.5;
         isJumping=false;
     }





     qDebug() << "velocityX:" << velocityX <<"VelocityY"<<velocityY<<"Gravity:"<<gravity<< "MaxSpeed:" << MaxSpeed << "acceleration:" << acceleration << "Deceleration"<<deceleration<<"raylength:" << A.distance<<"GroundAngle: "<<groundAngle;
     //qDebug()<<"A.hit:"<<A.hit<<"RayLength:"<<A.distance;
     //qDebug() << "Player pos:" << this->pos() << "leftFoot:" << leftFoot << "rightFoot:" << rightFoot;

}







