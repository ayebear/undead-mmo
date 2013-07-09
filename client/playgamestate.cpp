// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <ctime>
#include <string>
#include <sstream>
#include "playgamestate.h"
#include "packet.h"
#include "tile.h"

PlayGameState::PlayGameState(GameObjects& gameObjects): State(gameObjects)
{
    // Load character textures
    if (!playerTex.loadFromFile("data/images/characters/character.png"))
        exit(1);
    playerTex.setSmooth(true);
    if (!zombieTex.loadFromFile("data/images/characters/zombie.png"))
        exit(1);
    zombieTex.setSmooth(true);

    // TODO: Have it download the map from the server instead
    tileMap.loadMapFromFile("data/maps/2.map");
    Entity::setMapPtr(tileMap);

    theHud.chat.setNetManager(&objects.netManager);

    // TODO: Will need to send a request to the server (during or after the log-in process)
    // which will create a new entity on the server first, which gets a unique global ID,
    // and then that gets sent right back to the player who just logged in, and then
    // is allocated on the client.
    // Normally this would be called when a packet is received of type "new entity". And the ID would be received from the server.
    myPlayer = entList.add(Entity::Player, 1001);
    myPlayer->setTexture(playerTex);
    myPlayer->setPos(sf::Vector2f(objects.vidMode.width / 2, objects.vidMode.height / 2));

    // Add quite a few local test zombies for now
    for (int x = 2; x < 999; x++)
    {
        auto* zombie = entList.add(Entity::Zombie, x);
        zombie->setTexture(zombieTex);
        zombie->setPos(sf::Vector2f(rand() % tileMap.getMapWidth(), rand() % tileMap.getMapHeight()));
        zombie->setAngle(rand() % 360);
        zombie->setMoving(true);
        zombie->setSpeed(rand() % 50 + 25);
    }

    gameView.setSize(objects.window.getSize().x, objects.window.getSize().y);
    gameView.setCenter(myPlayer->getPos());

    theHud.setUp(gameObjects);

    playerInput.x = 0;
    playerInput.y = 0;

    playing = true;
    paused = false;
}

PlayGameState::~PlayGameState()
{
}

void PlayGameState::handleEvents()
{
    sf::Event event;
    while (objects.window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                action.exitGame();
                break;

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        if (theHud.chat.getInput())
                            theHud.chat.setInput(false);
                        else
                            action.popState();
                        break;

                    case sf::Keyboard::Return:
                        if (theHud.chat.getInput())
                            objects.netManager.sendChatMessage(theHud.chat.parseMessage());
                        theHud.chat.toggleInput();
                        break;

                    case sf::Keyboard::Key::P:
                        takeScreenshot();
                        break;

                    case sf::Keyboard::Key::Y:
                    {
                        // If player is not typing in the chat
                        if (!theHud.chat.getInput())
                        {
                            StateArgs someArgs;
                            someArgs.push_back("This is just a test error...");
                            action.pushState(StateType::Error, someArgs);
                        }
                        break;
                    }

                    default:
                        break;
                }
                theHud.chat.processInput(event.key.code);
                break;

            case sf::Event::MouseWheelMoved:
                theHud.chat.handleScrolling(event, objects.window);
                break;
            case sf::Event::TextEntered:
                theHud.chat.processTextEntered(event.text.unicode);
                break;

            case sf::Event::LostFocus:
                paused = true;
                break;

            case sf::Event::GainedFocus:
                paused = false;
                break;

            case sf::Event::Resized:
                handleWindowResized(objects);
                break;

            default:
                break;
        }
    }
    handleInput();

}

void PlayGameState::update()
{
    entList.update(elapsedTime);

    // Update the game view center position with the player's current position
    gameView.setCenter(myPlayer->getPos());

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

    if (viewCenter.x + (viewSize.x /2) >= tileMap.getMapWidth())
    {
        gameView.setCenter(tileMap.getMapWidth() - viewSize.x / 2, viewCenter.y);
        viewSize = gameView.getSize();
        viewCenter = gameView.getCenter();

    }
    if (viewCenter.y + (viewSize.y / 2) >= tileMap.getMapHeight())
    {
        gameView.setCenter(viewCenter.x, tileMap.getMapHeight() - viewSize.y  / 2);
        viewSize = gameView.getSize();
        viewCenter = gameView.getCenter();
    }

    theHud.update();
}

void PlayGameState::draw()
{
    objects.window.clear();

    // Use the game view to draw this stuff
    objects.window.setView(gameView);

	// Draw the tile map
    objects.window.draw(tileMap);

    // Draws all of the entities
    objects.window.draw(entList);

    // Draw the HUD (changes the window's view)
    objects.window.draw(theHud);

    objects.window.display();
}

void PlayGameState::handleInput()
{
    if (!paused && !theHud.chat.getInput())
    {
        // This is horrible code I wrote, we should make it better
        oldPlayerInput = playerInput;
        playerInput.x = 0;
        playerInput.y = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            playerInput.y--; // 90
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            playerInput.y++; // 270 (or -90)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            playerInput.x--; // 180
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            playerInput.x++; // 0 (or 360)

        // Calculate the degrees based on which keys were pressed
        float degrees = playerInput.y * 90;
        if (degrees != 0)
            degrees += -playerInput.y * playerInput.x * 45;
        else
            degrees = 90 - (90 * playerInput.x);

        if (playerInput != oldPlayerInput)
        {
            if (playerInput.x != 0 || playerInput.y != 0)
            {
                myPlayer->setAngle(degrees);
                myPlayer->setMoving(true);
            }
            else
            {
                degrees = 0;
                myPlayer->setMoving(false);
            }

            sf::Packet playerPacket;
            playerPacket << Packet::Input << myPlayer->isMoving() << degrees;
            objects.netManager.sendPacket(playerPacket);
        }
    }
    elapsedTime = clock.restart().asSeconds();
}

void PlayGameState::takeScreenshot()
{
    // If player is not typing in the chat
    if (!theHud.chat.getInput())
    {
        //Get the current system time.
        time_t currTime = time(0);
        string fileName = "data/screenshots/";
        stringstream ss;
        ss << currTime;

        //Add the time.png to the end of the file name and save it.
        fileName += ss.str() + ".png";
        sf::Image scrShot = objects.window.capture();
        scrShot.saveToFile(fileName);
    }
}

void PlayGameState::handleWindowResized(GameObjects& objects)
{
    sf::Vector2f windowSize;
    windowSize.x = objects.window.getSize().x;
    windowSize.y = objects.window.getSize().y;
    // Reset the view of the window
    gameView.setSize(windowSize);
    // objects.window.setView(gameView);
    viewDimensions = objects.window.getView().getSize();

    theHud.updateView(gameView, objects);
}
