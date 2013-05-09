// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "game.h"
#include "../shared/packet.h"
#include "../shared/tile.h"
#include <ctime>
#include <string>
#include <sstream>

const std::string version = "Project: Brains v0.0.1.6 Dev";

Game::Game()
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

    // TODO: Will need to send a request to the server (during or after the log-in process)
    // which will create a new entity on the server first, which gets a unique global ID,
    // and then that gets sent right back to the player who just logged in, and then
    // is allocated on the client.
    // Normally this would be called when a packet is received of type "new entity". And the ID would be received from the server.
    entList.Add(Entity::Player, 1001);

    // Add quite a few local test zombies for now
    for (int x = 2; x < 500; x++)
    {
        auto* zombie = entList.Add(Entity::Zombie, x);
        zombie->SetTexture(zombieTex);
        zombie->SetPos(sf::Vector2f(rand() % windowWidth, rand() % windowHeight));
        zombie->SetAngle(rand() % 360);
    }

    myPlayer = entList.Find(1001);
    myPlayer->SetTexture(playerTex);

    myPlayer->SetPos(sf::Vector2f(300, 400));

    // Create the window in fullscreen at max resolution
    vidMode = sf::VideoMode::getDesktopMode();
    window.create(vidMode, version, sf::Style::Fullscreen);

    // Create a normal window for now
    //vidMode = sf::VideoMode(windowWidth, windowHeight);
    //window.create(vidMode, version);

    gameView.setSize(vidMode.width, vidMode.height);
    gameView.setCenter(myPlayer->GetPos());

    theHud.UpdateView(gameView);

    // Set frame limits and vsync
    //window.setFramerateLimit(10);
    window.setVerticalSyncEnabled(true);

    playing = true;
    paused = false;

    netManager.LaunchThreads();
}

// TODO: Handle networking code! Receiving and sending with both TCP and UDP!
// Also integrate that with the chat class.
void Game::Start() // this will need to manage a second thread for the networking code
{
    Menu menu(window, vidMode);
    int choice = menu.processChoice(window);

    if(choice == 1)
    {
        gameView.setCenter(myPlayer->GetPos());
        window.setView(gameView);

        theHud.chat.PrintMessage("Warning: Currently not connected to a server! Please type '/help connect' for more info.", sf::Color::Yellow);

        sf::Clock clock;
        while (playing && window.isOpen())
        {
            //ReceiveData();
            ProcessEvents();
            ProcessInput();

            // Get the time since the last frame, and restart the timer
            elapsedTime = clock.restart().asSeconds();

            // This will update the positions and stuff of all of the sprites and logic
            Update();

            // This will render everything to the screen
            Display();
        }

    }
    //Change to whatever option quit is after we add more menu choices
    else if(choice == 2 || choice == 3)
    {
        return;
    }
}

void Game::ProcessEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        if (theHud.chat.GetInput())
                            theHud.chat.SetInput(false);
                        else
                            window.close();
                        break;
                    case sf::Keyboard::Return:
                        if (theHud.chat.GetInput())
                            netManager.SendChatMessage(theHud.chat.ParseMessage());
                        theHud.chat.ToggleInput();
                        break;

                    case sf::Keyboard::Key::P:
                        {
                            //Get the current system time.
                            time_t currTime = time(0);
                            string fileName = "screenshots/";
                            stringstream ss;
                            ss << currTime;

                            //Add the time.png to the end of the file name and save it.
                            fileName += ss.str() + ".png";
                            sf::Image scrShot = window.capture();
                            scrShot.saveToFile(fileName);
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
                // Minimum window size
                sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
                // Minimum size
                if(size.x < 800)
                    size.x = 800;
                if(size.y < 600)
                    size.y = 600;
                // This causes some issues to occur?
                //window.setSize(static_cast<sf::Vector2u>(size));
                // Reset the view of the window
                gameView.setSize(event.size.width, event.size.height);
                window.setView(gameView);
                theHud.chat.SetPosition(0, event.size.height - 182);
                break;
            }
            default:
                break;
        }
    }
}

void Game::ProcessInput()
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
}

void Game::Update()
{
    // All of the processing code will be run from here.

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

void Game::Display()
{
    window.clear();

    // Use the game view to draw this stuff
    window.setView(gameView);

	// Draw the tile map
    window.draw(tileMap);

    // Draws all of the entities
    window.draw(entList);

    // Draw the HUD (changes the window's view)
    window.draw(theHud);

    window.display();
}
