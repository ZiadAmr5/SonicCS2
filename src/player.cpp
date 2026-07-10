
#include <player.h>
#include <QDebug>
using namespace std;
Player::Player(QGraphicsItem *parent) : QGraphicsRectItem(parent),velocityX(0) ,velocityY(0){
    // Define a 40 width x 60 height rectangle placeholder
    setRect(0, 0, 40, 60);

    // Make it solid blue
    setBrush(QBrush(QColor(0, 100, 255)));
    setPen(QPen(Qt::NoPen)); // Removes the black outline border
    qDebug()<<"setting player in position.";
     this->setPos(400, 300);
    

}

 void Player:: physUpdate(bool right , bool left ,bool jump)
{// might include a case where both left and right are pressed , maybe a feature
     //qDebug()<<"calling the physics function";
     if(right)
    {

         //going left then pressing right
         if(velocityX<0)
         {
             velocityX+=deceleration*deltatime;

         if(velocityX>=0)
            {
             velocityX=0.5;
            }
         }
         //going right
         else if(velocityX<MaxSpeed)
         {
             velocityX+=acceleration*deltatime;
             if(velocityX>=MaxSpeed)
             {
                 velocityX=MaxSpeed;
             }
         }



     }
     else if (left)
     {
         if(velocityX>0)
         {
         velocityX-=deceleration*deltatime;
         if(velocityX<=0)
             {
             velocityX=-0.5;
             }
         }
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
     {

         velocityX-=min(abs(velocityX),friction)*sgn(velocityX);

     }
    qDebug() << "velocityX:" << velocityX << "MaxSpeed:" << MaxSpeed << "acceleration:" << acceleration << "Deceleration"<<deceleration<<"deltatime:" << deltatime;

     if(jump)
     {

     }

}




