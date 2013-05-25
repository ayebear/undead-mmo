// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "playgamestate.h"
#include "../shared/packet.h"
#include "../shared/tile.h"
#include <ctime>
#include <string>
#include <sstream>

PlayGameState PlayGameState::playState;

PlayGameState::PlayGameState()
{
}

void PlayGameState::init(GameEngine* game)
{


     // Load character sprites
    if (!playerTex.loadFromFile("data/images/characters/character.png"))
        exit(Errors::Graphics);
    playerTex.setSmooth(true);
    if (!zombieTex.loadFromFile("data/images/characters/zombie.png"))
        exit(Errors::Graphics);
    zombieTex.setSmooth(true);

    tileMap.LoadMapFromFile("data/maps/2.map");
    Entity::setMapPtr(tileMap);

    theHud.chat.SetNetManager(&game->netManager);

    // TODO: Will need to send a request to the server (during or after the log-in process)
    // which will create a new entity on the server first, which gets a unique global ID,
    // and then that gets sent right back to the player who just logged in, and then
    // is allocated on the client.
    // Normally this would be called when a packet is received of type "new entity". And the ID would be received from the server.
    myPlayer = entList.Add(Entity::Player, 1001);
    myPlayer->SetTexture(playerTex);
    myPlayer->SetPos(sf::Vector2f(300, 400));

    // Add quite a few local test zombies for now
    for (int x = 2; x < 50; x++)
    {
        auto* zombie = entList.Add(Entity::Zombie, x);
        zombie->SetTexture(zombieTex);
        zombie->SetPos(sf::Vector2f(rand() % tileMap.getMapWidth(), rand() % tileMap.getMapHeight()));
        zombie->SetAngle(rand() % 360);
    }


    gameView.setSize(game->window.getSize().x, game->window.getSize().y);
    gameView.setCenter(myPlayer->GetPos());

    theHud.UpdateView(gameView);

    playing = true;
    paused = false;

    //netManager.LaunchThreads();
}
void PlayGameState::cleanup()
{

}

void PlayGameState::pause()
{

}
void PlayGameState::resume()
{

}

void PlayGameState::handleEvents(GameEngine* game)
{
     sf::Event event;
    while (game->window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                game->quit();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        if (theHud.chat.GetInput())
                            theHud.chat.SetInput(false);
                        else
                            game->changeState(MainMenuState::instance());
                        break;
                    case sf::Keyboard::Return:
                        if (theHud.chat.GetInput())
                            game->netManager.SendChatMessage(theHud.chat.ParseMessage());
                        theHud.chat.ToggleInput();
                        break;

                    case sf::Keyboard::Key::P:
                        {
                            //If player is not typing
                            if(!theHud.chat.GetInput())
                            {
                                //Get the current system time.
                                time_t currTime = time(0);
                                string fileName = "data/screenshots/";
                                stringstream ss;
                                ss << currTime;

                                //Add the time.png to the end of the file name and save it.
                                fileName += ss.str() + ".png";
                                sf::Image scrShot = game->window.capture();
                                scrShot.saveToFile(fileName);
                            }
                        }
                    default:
                        break;
                }
                theHud.chat.ProcessInput(event.key.code);
                break;
            case sf::Event::TextEntered:
                // TODO: Make a function in the chat class that takes the event.text.unicode
                if (theHud.chat.GetInput() && event.text.unicode >= 32 && event.text.unicode <= 126)
                    theHud.chat.AddChar(static_cast<char>(event.text.unicode));
                break;
            case sf::Event::LostFocus:
                paused = true;
                break;
            case sf::Event::GainedFocus:
                paused = false;
                break;
            case sf::Event::Resized:
            {
                sf::Vector2f windowSize;
                windowSize.x = game->window.getSize().x;
                windowSize.y = game->window.getSize().y;
                // Reset the view of the window
                gameView.setSize(windowSize);
              //  game->window.setView(gameView);
                viewDimensions = game->window.getView().getSize();

                theHud.UpdateView(gameView);
                break;
            }
            default:
                break;
        }
    }
    ProcessInput();

}
void PlayGameState::update(GameEngine* game)
{
     // All of the processing code will be run from here.
   // cout << elapsedTime << endl;
    entList.Update(elapsedTime);

    // Update the game view center position with the player's current position
    gameView.setCenter(myPlayer->GetPos());

    sf::Vector2f viewSize(gameView.getSize());
    sf::Vector2f viewCenter(gameView.getCenter());
    if (viewCenter.x - (viewSize.x / 2) < 0)
    {
        gameView.setCenter(viewSize.x / 2, viewCenter.y);
        viewSize = gameView.getSize();
        viewCenter = gameView.getCenter();
    }
    if (viewCenter.y - (viewSize.y / 2) < 0)
    {
        gameView.setCenter(viewCenter.x, viewSize.y / 2);
        viewSize = gameView.getSize();
        viewCenter = gameView.getCenter();
    }

    if(viewCenter.x + (viewSize.x /2) >= tileMap.getMapWidth())
    {
        gameView.setCenter(tileMap.getMapWidth() - viewSize.x / 2, viewCenter.y);
        viewSize = gameView.getSize();
        viewCenter = gameView.getCenter();

    }
    if(viewCenter.y + (viewSize.y / 2) >= tileMap.getMapHeight())
    {
        gameView.setCenter(viewCenter.x, tileMap.getMapHeight() - viewSize.y  / 2);
        viewSize = gameView.getSize();
        viewCenter = gameView.getCenter();
    }

    theHud.Update();
}
void PlayGameState::draw(GameEngine* game)
{
    game->window.clear();

    // Use the game view to draw this stuff
    game->window.setView(gameView);

	// Draw the tile map
    game->window.draw(tileMap);

    // Draws all of the entities
    game->window.draw(entList);

    // Draw the HUD (changes the window's view)
    game->window.draw(theHud);

    game->window.display();
}

void PlayGameState::ProcessInput()
{
    if (!paused && !theHud.chat.GetInput())
    {
        // This is horrible code I wrote, we should make it better
        int x = 0;
        int y = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            y--; // 90
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            y++; // 270 (or -90)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            x--; // 180
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            x++; // 0 (or 360)
        float degrees = y * 90;
        if (degrees != 0)
            degrees += -y * x * 45;
        else
            degrees = 90 - (90 * x);
        if (x != 0 || y != 0)
            myPlayer->SetAngle(degrees);
    }
    elapsedTime = clock.restart().asSeconds();
}
// TODO: Make a game state manager with different game states


