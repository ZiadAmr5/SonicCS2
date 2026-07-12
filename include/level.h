#ifndef LEVEL_H
#define LEVEL_H
<<<<<<< HEAD

#include <QPixmap>
#include <QString>
#include <QVector>
=======
#include <QPixmap>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QLineF>
>>>>>>> eb1a88bea16188b20eb1593eb07cc46ffd7e865d
#include <QMap>
struct collisionBoundary
{
    qreal x,y,w,l;
<<<<<<< HEAD
    QMap<QString,QString> properties;
=======
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
    
    
>>>>>>> eb1a88bea16188b20eb1593eb07cc46ffd7e865d
};

class level
{
<<<<<<< HEAD

    int widthpx =0;
    int lengthpx =0;

public:

    level parse(QString& path);

=======
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
>>>>>>> eb1a88bea16188b20eb1593eb07cc46ffd7e865d

};

#endif // LEVEL_H
