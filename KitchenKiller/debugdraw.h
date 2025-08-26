#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <Box2D/Box2D.h>
#include <QWidget>
#include <QPainter>

/**
 * @brief The DebugDraw class This is a class that draws different shapes in cooking stage for debugging.
 */
class DebugDraw : public b2Draw
{
public:

    /**
     * @brief DebugDraw Constructor
     */
    DebugDraw(QWidget*);

    /**
     * @brief DrawPolygon Draws a polygon
     * @param vertices The vertices that make up the polygon
     * @param vertexCount The number of vertex
     * @param color The color of the shape.
     */
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    /**
     * @brief DrawSolidPolygon Draws a solid Polygon
     * @param vertices The vertices that make up the polygon
     * @param vertexCount The number of vertex
     * @param color The color of the shape.
     */
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    /**
     * @brief DrawCircle Draws a circle
     * @param center The center of the circle
     * @param radius The radius of the circle
     * @param color The color of the circle
     */
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

    /**
     * @brief DrawSolidCircl2 Draws a solid circle
     * @param center The center of the circle
     * @param radius The radius of the circle
     * @param axis The axis of the circle
     * @param color The color of the circle
     */
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

    /**
     * @brief DrawSegment Draws a segment
     * @param p1 The start of the segment in b2Vector
     * @param p2 The end of the segment in b2Vector
     * @param color The color of the segment
     */
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

    /**
     * @brief DrawTransform Draws a transform
     * @param xf b2TransformSS
     */
    void DrawTransform(const b2Transform& xf);

private:
    QWidget *widget=nullptr;
};

#endif // DEBUGDRAW_H
