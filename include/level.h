#ifndef LEVEL_H
#define LEVEL_H

#include <QPixmap>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QMap>
#include <QGraphicsScene>  // 🌟 Added for UI Scene parsing
#include <QGraphicsItem>   // 🌟 Added to inspect visual objects

struct collisionBoundary {
    qreal x, y, w, l;
    QString name;
    QMap<QString, QString> properties;
    enum shapeType { Rectangle, Polygon };

    QVector<QPointF> points;
    shapeType shape = Rectangle;

    QVector<QLineF> edges() {
        QVector<QLineF> result;
        if (shape == Rectangle) {
            QPointF topLeft(x, y);
            QPointF topRight(x + w, y);
            QPointF botRight(x + w, y + l);
            QPointF botLeft(x, y + l);

            result = {
                QLineF(topLeft, topRight),
                QLineF(topRight, botRight),
                QLineF(botRight, botLeft),
                QLineF(botLeft, topLeft)
            };
        }
        else if (shape == Polygon) {
            for (int i = 0; i < points.size(); i++) {
                QPointF a = points[i];
                QPointF b = points[(i + 1) % points.size()];
                result.append(QLineF(a, b));
            }
        }
        return result;
    }
};

class level {
    int widthpx = 0;
    int lengthpx = 0;
    QVector<collisionBoundary> collisionBoundaries;
    QPixmap m_map;

public:
    // 🌟 CHANGED: Instead of a TMX path, we load straight from your visual UI scene
    void loadFromUiScene(QGraphicsScene* scene);

    int getwidthPx() const { return widthpx; }
    int getlengthPx() const { return lengthpx; }
    QPixmap getMap() const { return m_map; }
    QVector<collisionBoundary>& getcollisionBoundaries() { return collisionBoundaries; }
};

#endif // LEVEL_H
