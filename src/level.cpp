#include <level.h>
#include <QFile>
#include <QXmlStreamReader>
#include <QLatin1StringView>
#include <QDebug>

level level::load(const QString& tmxPath,const QString& imgPath)
{
    level lev;
    lev.m_map=  QPixmap(imgPath);
    QFile file(tmxPath);

    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        qWarning()<<"Could not open tmx file"<<tmxPath;
        return lev;
    }

    QXmlStreamReader xml(&file);
    int tileWidth;
    int tileLength;
    int mapWidthTiles;
    int mapLengthTiles;
    while(!xml.atEnd()&&!xml.hasError())
    {
        xml.readNext();
        if(!xml.isStartElement())
        {
            continue;
        }
        if(xml.name()==QLatin1StringView("map"))
        {
            tileWidth= xml.attributes().value("tilewidth").toInt();
            tileLength= xml.attributes().value("tileheight").toInt();
            mapWidthTiles = xml.attributes().value("width").toInt();
            mapLengthTiles=xml.attributes().value("height").toInt();
           lev.widthpx = tileWidth*mapWidthTiles;
           lev.lengthpx= tileLength*mapLengthTiles;
        }

        else if(xml.name()==QLatin1StringView("objectgroup"))
        {
            while(!(xml.isEndElement()&&xml.name()==QLatin1StringView("objectgroup")))
            {
                xml.readNext();
                if(xml.isStartElement()&&xml.name()==QLatin1StringView("object"))
                {
                 collisionBoundary b;
                    b.w =xml.attributes().value("width").toDouble();
                 b.l=xml.attributes().value("height").toDouble();
                    b.name=xml.attributes().value("name").toString();
                 b.x= xml.attributes().value("x").toDouble();
                    b.y = xml.attributes().value("y").toDouble();
                 //this loop and its condition is for when properties of the objects in the level are added
                    while(!(xml.isEndElement()&&xml.name()==QLatin1StringView("object")))
                    {
                        xml.readNext();
                         if(xml.isStartElement()&&xml.name()==QLatin1StringView("property"))
                        {

                        }
                    }
                 lev.collisionBoundaries.append(b);
                }
            }
        }
    }
    if(xml.hasError())
        qWarning()<<"Parsing error";
    return lev;
}
