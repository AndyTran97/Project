#include "shape.h"
#include "qdebug.h"


map<b2Body*, Shape*> Shape::shapes = map<b2Body*, Shape*>();

Shape::Shape(b2World* world, const b2Vec2& position, const b2Vec2& size, float32 density, float32 friction, QObject* parent)
    : QObject(parent),
    m_size(size)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position;

    b2PolygonShape shape;
    shape.SetAsBox(size.x, size.y);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;

    qDebug() << world->IsLocked();

    m_body = nullptr;
    m_body = world->CreateBody(&bodyDef);
    m_body->CreateFixture(&fixtureDef);
    m_body->SetUserData(this);

    shapes[m_body] = this;
    //m_body->SetUserData(m_data);
}

Shape::~Shape() {
    for (b2JointEdge* edge = m_body->GetJointList(); edge; edge = edge->next) {
        b2Joint* joint = edge->joint;

        bool* destroyed = new bool(true);
        joint->SetUserData(destroyed);

        m_body->GetWorld()->DestroyJoint(joint);
    }
    m_body->SetUserData(nullptr);
    m_body->GetWorld()->DestroyBody( m_body );
    m_body = nullptr;
    shapes.erase(m_body);
}


void Shape::setStatic(bool value) {
    b2BodyType bodyType = value ? b2_staticBody : b2_dynamicBody;
    m_body->SetType(bodyType);
}
