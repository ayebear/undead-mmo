// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PLAYER_H
#define PLAYER_H

#include "mobileentity.h"

class Player: public MobileEntity
{
    public:
        Player();
        void update(float);
        bool collides(Entity*);
        void draw(sf::RenderTarget&, sf::RenderStates) const;
        void getData(sf::Packet&);
        void setData(sf::Packet&);
        float getVisualAngle() const;
        void setVisualAngle(float);
        void updateSpriteRotation();

    private:
        float visualAngle;
};

#endif
