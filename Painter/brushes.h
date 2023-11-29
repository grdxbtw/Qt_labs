#ifndef BRUSHES_H
#define BRUSHES_H

#endif // BRUSHES_H

#include <QtWidgets>

class CurveBrush : public QPainter
{
public:
    CurveBrush(QWidget *widget, QColor color, int thickness)
        : QPainter(widget)
    {
        setPen(QPen(color, thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }

    void drawCurve(const QPolygonF &points)
    {
        drawPolyline(points);
    }
};

class CircleBrush : public QPainter
{
public:
    CircleBrush(QWidget *widget, QColor fillColor, QColor borderColor, int diameter)
        : QPainter(widget), fillColor(fillColor), borderColor(borderColor), diameter(diameter) {
        setRenderHint(QPainter::Antialiasing);
    }

    void drawCircle()
    {
        drawEllipse(QPointF(0, 0), diameter / 2, diameter / 2);
        fillPath(createCirclePath(), fillColor);
        setPen(QPen(borderColor, 1, Qt::SolidLine));
        drawEllipse(QPointF(0, 0), diameter / 2, diameter / 2);
    }

private:
    QPainterPath createCirclePath() const
    {
        QPainterPath path;
        path.addEllipse(QPointF(0, 0), diameter / 2, diameter / 2);
        return path;
    }

private:
    QColor fillColor;
    QColor borderColor;
    int diameter;
};

class PolygonBrush : public QPainter
{
public:
    PolygonBrush(QWidget *widget, QColor fillColor, QColor borderColor, int diameter)
        : QPainter(widget), fillColor(fillColor), borderColor(borderColor), diameter(diameter)
    {
        setRenderHint(QPainter::Antialiasing);
    }

    void drawPolygon()
    {
        drawConvexPolygon(createPolygon());
        fillPath(createPolygonPath(), fillColor);
        setPen(QPen(borderColor, 1, Qt::SolidLine));
        drawConvexPolygon(createPolygon());
    }

private:
    QPolygonF createPolygon() const
    {
        return generateRegularPolygon(6, diameter / 2);
    }

    QPainterPath createPolygonPath() const
    {
        QPainterPath path;
        path.addPolygon(createPolygon());
        return path;
    }

    QPolygonF generateRegularPolygon(int sides, qreal radius) const
    {
        QPolygonF polygon;
        for (int i = 0; i < sides; ++i)
        {
            qreal angle = i * 2.0 * M_PI / sides;
            qreal x = radius * cos(angle);
            qreal y = radius * sin(angle);
            polygon << QPointF(x, y);
        }
        return polygon;
    }

private:
    QColor fillColor;
    QColor borderColor;
    int diameter;
};

