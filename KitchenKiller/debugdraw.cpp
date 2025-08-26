#include "debugdraw.h"

DebugDraw::DebugDraw(QWidget *widget) : widget(widget)
{

}


void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    // Draw the polygon using Qt's QPainter
    QPainter painter(widget);

    painter.setPen(QPen(QColor(color.r * 255, color.g * 255, color.b * 255), 1));
    painter.setBrush(Qt::NoBrush);

    QVector<QPointF> points;
    for (int i = 0; i < vertexCount; i++)
    {
        points.append(QPointF(vertices[i].x, vertices[i].y));
    }

    painter.drawPolygon(QPolygonF(points));

    painter.end();
}


void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    QPainter painter(widget);

    painter.setPen(QPen(QColor(color.r * 255, color.g * 255, color.b * 255), 1));

    QColor c(Qt::black);
    c.setAlpha(32);
    QBrush brush(c, Qt::SolidPattern);
    painter.setBrush(brush);

    QVector<QPointF> points;
    for (int i = 0; i < vertexCount; i++)
    {
        points.append(QPointF(vertices[i].x, vertices[i].y));
    }

    painter.drawPolygon(QPolygonF(points));

    painter.end();
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    // Implement drawing code here
    qDebug() << "drawing";
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    // Implement drawing code here
    qDebug() << "drawing";
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    // Implement drawing code here
    qDebug() << "drawing";
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
    // Implement drawing code here
    qDebug() << "drawing";
}
