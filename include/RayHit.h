#ifndef RAYHIT_H
#define RAYHIT_H
#include <QPointF>
#include<QLineF>
//this structure is now unused
struct RayHit
{
    bool hit=false;
    qreal distance=0;
    QPointF point;
    QLineF edge;

};

#endif // RAYHIT_H
