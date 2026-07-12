#ifndef LEVEL_H
#define LEVEL_H

#include <QPixmap>
#include <QString>
#include <QVector>
#include <QMap>
struct collisionBoundary
{
    qreal x,y,w,l;
    QMap<QString,QString> properties;
};

class level
{

    int widthpx =0;
    int lengthpx =0;

public:

    level parse(QString& path);


};

#endif // LEVEL_H
