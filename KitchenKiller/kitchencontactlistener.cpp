#include "kitchencontactlistener.h"
#include "shape.h"

void KitchenContactListener::BeginContact(b2Contact* contact) {
    // This function is called when two shapes begin to touch each other.
    // You can use it to detect when a dynamic body collides with a static body.

    // Get the two colliding shapes.
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    // Get the bodies that the shapes are attached to.
    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    bool staticA = bodyA->GetType() == b2_staticBody;
    bool staticB = bodyB->GetType() == b2_staticBody;

    // Check if one of the bodies is the static body you're interested in.
    if (staticA || staticB) {

        Shape* shapeA = static_cast<Shape*>(bodyA->GetUserData());
        Shape* shapeB = static_cast<Shape*>(bodyB->GetUserData());

        if (staticA)
            emit shapeA->onContact(shapeB);
        else
            emit shapeB->onContact(shapeA);
    }
}


