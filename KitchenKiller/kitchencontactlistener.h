#ifndef KITCHENCONTACTLISTENER_H
#define KITCHENCONTACTLISTENER_H

#include <Box2D/Box2D.h>

/**
 * @brief The KitchenContactListener class Represents a event listener to b2 object contact
 */
class KitchenContactListener : public b2ContactListener
{
public:

    /**
     * @brief BeginContact 2 b2 objects contact each otherS
     * @param contact b2Contsct pointer
     */
    virtual void BeginContact(b2Contact* contact);
};

#endif // KITCHENCONTACTLISTENER_H
