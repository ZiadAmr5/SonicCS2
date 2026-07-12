#include <level.h>
#include <QFile>
#include <QXmlStreamReader>
#include <QLatin1StringView>
#include <QDebug>

level level::load(const QString& tmxPath, const QString& imgPath)
{
    level lev;
    lev.m_map = QPixmap(imgPath);
    QFile file(tmxPath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Could not open tmx file" << tmxPath;
        return lev;
    }

    QXmlStreamReader xml(&file);
    int tileWidth = 0;
    int tileLength = 0;
    int mapWidthTiles = 0;
    int mapLengthTiles = 0;

    while(!xml.atEnd() && !xml.hasError())
    {
        xml.readNext();

        // Safe evaluation pattern: check states explicitly on the current token
        if(xml.isStartElement())
        {
            if(xml.name() == QLatin1StringView("map"))
            {
                tileWidth = xml.attributes().value("tilewidth").toInt();
                tileLength = xml.attributes().value("tileheight").toInt();
                mapWidthTiles = xml.attributes().value("width").toInt();
                mapLengthTiles = xml.attributes().value("height").toInt();
                lev.widthpx = tileWidth * mapWidthTiles;
                lev.lengthpx = tileLength * mapLengthTiles;
            }
            else if(xml.name() == QLatin1StringView("objectgroup"))
            {
                while(!xml.atEnd() && !xml.hasError())
                {
                    xml.readNext();

                    if(xml.isEndElement() && xml.name() == QLatin1StringView("objectgroup"))
                    {
                        break;
                    }

                    if(xml.isStartElement() && xml.name() == QLatin1StringView("object"))
                    {
                        collisionBoundary b;
                        b.w = xml.attributes().value("width").toDouble();
                        b.l = xml.attributes().value("height").toDouble();
                        b.name = xml.attributes().value("name").toString();
                        b.x = xml.attributes().value("x").toDouble();
                        b.y = xml.attributes().value("y").toDouble();
                        b.shape = collisionBoundary::Rectangle;

                        while (!xml.atEnd() && !xml.hasError())
                        {
                            xml.readNext();

                            if (xml.isEndElement() && xml.name() == QLatin1StringView("object")) {
                                break;
                            }

                            if (xml.isStartElement())
                            {
                                if (xml.name() == QLatin1StringView("polygon") || xml.name() == QLatin1StringView("polyline"))
                                {
                                    b.shape = collisionBoundary::Polygon;
                                    QString pointsAttr = xml.attributes().value("points").toString();
                                    QStringList pointPairs = pointsAttr.split(QLatin1Char(' '));

                                    for (const QString& pair : pointPairs)
                                    {
                                        QStringList coords = pair.split(QLatin1Char(','));
                                        if (coords.size() == 2)
                                        {
                                            qreal posX = b.x + coords[0].toDouble();
                                            qreal posY = b.y + coords[1].toDouble();
                                            b.points.append(QPointF(posX, posY));
                                        }
                                    }
                                }
                                else if (xml.name() == QLatin1StringView("property"))
                                {
                                    QString propName = xml.attributes().value("name").toString();
                                    QString propValue = xml.attributes().value("value").toString();
                                    b.properties.insert(propName, propValue);
                                }
                            }
                        }
                        lev.collisionBoundaries.append(b);
                    }
                }
            }
        }
    }

    if(xml.hasError())
        qWarning() << "Parsing error:" << xml.errorString();

    return lev;
}
