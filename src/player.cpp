
#include <player.h>
#include <QDebug>
using namespace std;
//quick player rectangle for testing ( I think it is obvious where this came from, except velocityX and y)
Player::Player(QGraphicsItem *parent) : QGraphicsRectItem(parent),velocityX(0) ,velocityY(0){
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
 void Player:: physUpdate(bool right , bool left ,bool jump,RayHit A,RayHit B)
{// might include a case where both left and right are pressed , maybe a feature
     //qDebug()<<"calling the physics function";
     if(right)
    {

         //going left then pressing right
         if(velocityX<0)
         {
             velocityX+=deceleration*deltatime; //scale per deltatime

         if(velocityX>=0)
            {
             velocityX=0.5;  //this makes it so that when the player turns, they would start with a speed = 0.5
            }
         }
         //going right
         else if(velocityX<MaxSpeed)
         {
             velocityX+=acceleration*deltatime;
             if(velocityX>=MaxSpeed) //this is a speed cap, should be removed later on when slopes are implemented
             {
                 velocityX=MaxSpeed;
             }
         }



     }
     else if (left)
     {//going right then pressing left
         if(velocityX>0)
         {
         velocityX-=deceleration*deltatime;
         if(velocityX<=0)
             {
             velocityX=-0.5;
             }
         }
         //going left
         else if(velocityX>-MaxSpeed)
         {
             velocityX-=acceleration*deltatime;
             if(velocityX<=-MaxSpeed)
             {
                 velocityX=-MaxSpeed;
             }
         }

     }
     else
         //apply friction, we subract from the smallest value  so we don't go past zero
     {

         velocityX-=min(abs(velocityX),friction)*sgn(velocityX);

     }


         //qDebug()<<"Falling to the ground";
         if(A.hit||B.hit)
         {
             //qDebug()<<"Hit Confirmed";
             RayHit ground =(A.hit&&(!B.hit||A.distance<B.distance))? A:B;
             isOnGround=true;
             velocityY=0;
             this->setPos(this->pos().x(),ground.point.y()-60);

         }

     else
     {
         isOnGround = false;
         velocityY+=gravity*deltatime;
           if(abs(velocityY)>MaxSpeed)
             velocityY=MaxSpeed;
     }

//qDebug() << "velocityX:" << velocityX <<"VelocityY"<<velocityY<<"Gravity:"<<gravity<< "MaxSpeed:" << MaxSpeed << "acceleration:" << acceleration << "Deceleration"<<deceleration<<"raylength:" << A.distance;
     qDebug()<<"A.hit:"<<A.hit<<"RayLength:"<<A.distance;
     qDebug() << "Player pos:" << this->pos() << "leftFoot:" << leftFoot << "rightFoot:" << rightFoot;

}







