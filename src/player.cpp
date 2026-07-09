
#include <player.h>

Player::Player(QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    // Define a 40 width x 60 height rectangle placeholder
    setRect(0, 0, 40, 60);

    // Make it solid blue
    setBrush(QBrush(QColor(0, 100, 255)));
    setPen(QPen(Qt::NoPen)); // Removes the black outline border
}

 void Player:: physUpdate(bool right , bool left ,bool jump)
{// might include a case where both left and right are pressed , maybe a feature
     if(right)
    {
        if(velocityX<MaxSpeed)
         {
             velocityX+=accelration;

        }
     }
     else if (left)
     {
         if(velocityX!=0)
         velocityX-=accelration;
     }
     else
     {
         if(velocityX>0)
         velocityX-=friction;
         else
             velocityX+=0;
     }

     if(jump)
     {

     }
}
void Player:: keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::RightArrow)
    {
        rightKeyPressed= true;
         sendHorizontalSpeed(velocityX);
    }
    else if (event->key() == Qt::LeftArrow)
    {
        leftKeyPressed=true;
        sendHorizontalSpeed(velocityX);
    }

}

