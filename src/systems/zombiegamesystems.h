#ifndef ZOMBIEGAMESYSTEMS_H
#define ZOMBIEGAMESYSTEMS_H

#include <OCS/System.hpp>

struct MovementSystem : public ocs::System
{
    void update(ObjectManager&, MessageHub&, double);
};

struct CollisionSystem : public ocs::System
{ 
    void handleCollisions(ObjectManager&, MessageHub&, double);
    void update(ObjectManager&, MessageHub&, double);
};

struct RenderingSystem : public ocs::System
{ 
    void update(ObjectManager&, MessageHub&, double);
};

struct NetworkSystem : public ocs::System
{ 
    void update(ObjectManager&, MessageHub&, double);
};

struct InfectionSystem : public ocs::System
{
    void update(ObjectManager&, MessageHub&, double);
};

#endif


/*************Delete**
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                stop();
                break;
            default:
                msgHub.postMessage<InputOccurred>(*this, event);
                break;
        }
    }

   struct WindowMessage : public ocs::Message<WindowMessage>
   {
        WindowMessage(dfsdf): window(dfgsdf); {}
        sf::RenderWindow& window;
   };

   *********************Delete**/
