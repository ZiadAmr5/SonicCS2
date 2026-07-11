#ifndef LEVEL_H
#define LEVEL_H
#include <QPixmap>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QMap>
struct collisionBoundary
{
    qreal x,y,w,l;
    QString name; // name of the object we check for collision
    QMap<QString,QString> properties;
    enum shapeType{Rectangle,Polygon};

    QVector<QPointF> points;
    shapeType shape = Rectangle;
    QVector<QLineF> edges()
    {
        QVector <QLineF> result;
        if(shape ==Rectangle)
        {
            QPointF topRight(x+l,y);
            QPointF topLeft(x,y);
            QPointF botRight(x,y+w);
            QPointF botLeft(x+l,y+w);

            result={topRight,topLeft,botRight,botLeft};
        }

        else if (shape==Polygon)
        {
            for(int i=0;i<points.size();i++)
            {
                QPointF a = points[i];
                QPointF b = points[i+1]%points.size; //circle back to the first point
                result.append(QLineF(a,b));
            }

        }
        return result;
    }
    
    
};

class level
{
    int widthpx=0;
    int lengthpx=0;
    QVector<collisionBoundary> collisionBoundaries;
    QPixmap m_map;
    
public:
   
    level load(const QString& tmxPath,const QString& imgPath); // path to the tmx and the png
    int getwidthPx()
    {
        return widthpx;
    }
    int getlengthPx()
    {
        return lengthpx;
    }
    QPixmap getMap()const
    {
        return m_map;
    }
    QVector<collisionBoundary> getcollisionBoundaries() const
    {
        return collisionBoundaries;
    }

};

#endif // LEVEL_H
