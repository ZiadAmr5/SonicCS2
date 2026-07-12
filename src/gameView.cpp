#include "GameEngine.h"

gameView::gameView(Player* p ,level* l,QWidget* parent ):scene(nullptr), mp(p),QGraphicsView(parent),leftKeyPressed(false),rightKeyPressed(false),DownKeyPressed(false),UpKeyPressed(false)
{
    scene=new QGraphicsScene(this);
    scene->setSceneRect(0,0,800,600);
    setFocusPolicy(Qt::StrongFocus);
    ml=l;
    //load returns a copy of level, so we make ml point to said copy
    *ml=ml->load(":/assets/assets/testplatform.tmx",":/assets/assets/testplatform.png");
    renderLevel(*ml);
    scene-> addItem(mp);
    scaleFactor = scene->width()/320.0; //scale factor is unsed for now
    mp->getScaleFactor(scaleFactor);
    setScene(scene);


}
void gameView::keyPressEvent(QKeyEvent* event)
{
    if(event->key()==Qt::Key_Right)
    {
        rightKeyPressed=true;
        pressedKeys.insert(event->key());
    }
    else if(event->key()==Qt::Key_Left)
    {
        leftKeyPressed=true;
        pressedKeys.insert(event->key());
    }

}
void gameView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Right)
    {

        rightKeyPressed=false;
        pressedKeys.remove((event->key()));


    }
    else if(event->key()==Qt::Key_Left)
    {
        leftKeyPressed=false;
        pressedKeys.remove(event->key());

    }
}
void gameView::updatePosition()
{
    float vx= mp->getHorizontalSpeed();
    float vy=mp->getVerticalSpeed();
    mp->setPos(mp->pos().x()+vx*deltatime,mp->pos().y()+vy*deltatime);
}
void gameView::renderLevel(const level& lev)
{
    scene->addPixmap(lev.getMap());
    for(collisionBoundary& b:lev.getcollisionBoundaries() )
    {
       auto* item = new QGraphicsRectItem(0,0,b.w,b.l);
        item->setPos(b.x,b.y);
       scene->addItem(item);
    }
}

gameLoop::gameLoop(gameView* gv,Player* p,level* l):m_gv(gv),m_p(p),m_l(l)
{
    frameRate.start(16); // 1frame = 16 ms to achieve 60 fps
    TimeBetFrames.start();
    connect(&frameRate,&QTimer::timeout,this,&gameLoop::gameTick);


}


void gameLoop::gameTick()
{
    double deltatime= TimeBetFrames.restart()/1000.0;
    m_p->setDeltaTime(deltatime);
    m_gv->setDeltaTime(deltatime);
    qreal sensorLength = std::max(10.0, std::abs(m_p->getVerticalSpeed() * deltatime) + 5);
    RayHit left=GroundSensor(m_l,m_p->getleftSensor(),sensorLength);
   RayHit right= GroundSensor(m_l,m_p->getrightSensor(),sensorLength);
    m_p->physUpdate(m_gv->getrightKey(),m_gv->getleftKey(),m_gv->getJump(),left,right);
    m_gv->updatePosition();
     m_p->updateSensor();


}
RayHit gameLoop::castRayAgainistEdges(const QLineF ray,QVector<QLineF> edges)
{
    RayHit result;
    qreal closestDistance=ray.length();
    for(const QLineF edge:edges)
    {
        QPointF intersection;
        if(ray.intersects(edge,&intersection)==QLineF::BoundedIntersection)
        {
            QLineF distance(ray.p1(),intersection);
            qreal dist =distance.length() ;
            if(dist<closestDistance)
            {
                closestDistance=dist; //the new closest distance for future comparisons aganist other edges
                result.edge=edge;
                result.point=intersection;
                result.distance=dist;

                result.hit=true;
            }

        }
    }
    //qDebug()<<"collision detected";
    return result;

}
RayHit gameLoop::GroundSensor(level* l ,QPointF origin,qreal endPoint)
{
    QLineF ray(origin ,QPointF(origin.x(),origin.y()+endPoint));
    RayHit closest;
    qreal closestDist=endPoint;
    for(collisionBoundary& b:l->getcollisionBoundaries())
    {
        qDebug() << "Rect:" << b.name << b.x << b.y << b.w << b.l;
        RayHit hit= castRayAgainistEdges(ray,b.edges());
        if(hit.hit&&hit.distance<closestDist)
        {
            closestDist= hit.distance;
            closest=hit;
        }
    }
    return closest;
}



