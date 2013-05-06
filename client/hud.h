#ifndef HUD_H
#define HUD_H

#include "SFML/Graphics.hpp"

#include "chat.h"

class Hud: public sf::Drawable
{

  public:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    Hud();

    void fixViews(sf::View);

    Chat getChat();


  private:

    sf::Font font, fontBold, fontMono, fontMonoBold;
     //For reference
    sf::View gameView;

    sf::View chatView;
    Chat chat;

    //Add view for health/infection bars here.

    //Add view for mini-map

};
#endif // HUD_H
