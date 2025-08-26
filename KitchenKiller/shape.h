#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <map>
#include <Box2D/Box2D.h>

using std::map;
using std::unique_ptr;


class Shape : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Creates a box shape based on passed parameters.
     *
     * Creates a box shape based on the passed position, size, density, and friction.
     * Also adds it to a static map for easy parsing.
     *
     * @param world         the world to create the b2Body
     * @param position      the position of the b2Body
     * @param size          the size of the box
     * @param density       the density
     * @param friction      the friction
     * @param parent
     */
    explicit Shape(b2World *world, const b2Vec2& position, const b2Vec2& size, float32 density = 0.5f, float32 friction = 0.3f, QObject *parent = nullptr);
    ~Shape();

    // contains map of shapes that are in the world
    // through constructor and deconstructor
    static map<b2Body*, Shape*> shapes;

    /**
     * @brief Returns the b2Body binded to the shape.
     *
     * @return              the b2Body
     */
    b2Body* getBody() const { return m_body; }

    /**
     * @brief Returns a pointer.
     *
     * @return              the pointer
     */
    void* getData() { return m_data; }

    /**
     * @brief Returns the size binded to the shape.
     *
     * @return              the b2Vec2 size
     */
    b2Vec2 getSize() const { return m_size; }

    /**
     * @brief Sets the shape to either static or dynamic.
     *
     * @param value         static if true, dynamic otherwise
     */
    void setStatic(bool value);

    /**
     * @brief Sets the pointer.
     *
     * @param               the pointer
     */
    void setData(void* data) { m_data = data; }

private:
    b2Body* m_body=nullptr;
    b2Vec2 m_size;

    void* m_data;

signals:
    void onContact(Shape*);

};


#endif // SHAPE_H
