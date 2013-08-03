// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "playgamestate.h"
#include <ctime>
#include <string>
#include <sstream>
#include "packet.h"
#include "tile.h"

PlayGameState::PlayGameState(GameObjects& gameObjects): State(gameObjects)
{
    // TODO: Have it download the map from the server instead
    tileMap.loadMapFromFile("data/maps/2.map");
    Entity::setMapSize(tileMap.getMapWidth(), tileMap.getMapHeight());

    //theHud.chat.setNetManager(&objects.netManager);

    // TODO: Will need to send a request to the server (during or after the log-in process)
    // which will create a new entity on the server first, which gets a unique global ID,
    // and then that gets sent right back to the player who just logged in, and then
    // is allocated on the client.
    // Normally this would be called when a packet is received of type "new entity". And the ID would be received from the server.
    //myPlayer = entList.add(Entity::Player, 1);
    //myPlayer->setTexture(playerTex);
    //myPlayer->setPos(sf::Vector2f(objects.vidMode.width / 2, objects.vidMode.height / 2));
    myPlayer = nullptr;
    myPlayerId = 0;
    playerIsMoving = false;

    // Add quite a few local test zombies for now
    /*for (int x = 2; x < 999; x++)
    {
        auto* zombie = entList.add(Entity::Zombie, x);
        zombie->setTexture(zombieTex);
        zombie->setPos(sf::Vector2f(rand() % tileMap.getMapWidth(), rand() % tileMap.getMapHeight()));
        zombie->setAngle(rand() % 360);
        zombie->setMoving(true);
        zombie->setSpeed(rand() % 500 + 200);
        zombie->moveTo(sf::Vector2f(500, 500));
    }*/

    gameView.setSize(objects.window.getSize().x, objects.window.getSize().y);
    //gameView.setCenter(myPlayer->getPos());

    theHud.setUp(gameObjects);

    playerInput.x = 0;
    playerInput.y = 0;
    currentAngle = 0;
    lastSentAngle = 0;

    playing = true;
    hasFocus = false;
    mouseMoved = false;
}

PlayGameState::~PlayGameState()
{
}

void PlayGameState::processArgs(const StateArgs& args)
{
    // Nothing for now, possibly will use this in the future for logging in or something
}

void PlayGameState::onPush()
{
    theHud.chat.clear();
    theHud.chat.setUsername(objects.netManager.getUsername());
}

void PlayGameState::onPop()
{
    objects.netManager.logOut();
    myPlayer = nullptr;
    myPlayerId = 0;
    entList.clear();
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
                            theHud.chat.parseMessage();
                        theHud.chat.toggleInput();
                        break;

                    case sf::Keyboard::Key::F1:
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

            case sf::Event::MouseButtonPressed:
                theHud.chat.handleMouseClicked(event, objects.window);
                break;

            case sf::Event::MouseWheelMoved:
                theHud.chat.handleScrolling(event, objects.window);
                break;

            case sf::Event::TextEntered:
                theHud.chat.processTextEntered(event.text.unicode);
                break;

            case sf::Event::MouseMoved:
                mouseMoved = true;
                theHud.handleMouseMoved(event, objects.window);
                break;

            case sf::Event::LostFocus:
                hasFocus = true;
                break;

            case sf::Event::GainedFocus:
                hasFocus = false;
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
    processEntityPackets();

    if (myPlayer == nullptr)
        processPlayerIdPackets();

    entList.update(elapsedTime);

    if (mouseMoved)
    {
        handleMouseInput();
        mouseMoved = false;
    }

    sendAngleInputPacket();

    // Update the game view center position with the player's current position
    if (myPlayer != nullptr)
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
    if (!hasFocus && !theHud.chat.getInput())
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
            sf::Packet inputPacket;
            inputPacket << Packet::Input;

            if (playerInput.x != 0 || playerInput.y != 0)
            {
                //myPlayer->setAngle(degrees);
                //myPlayer->setMoving(true);
                playerIsMoving = true;
                inputPacket << Packet::InputType::StartMoving << degrees;
            }
            else
            {
                //myPlayer->setMoving(false);
                playerIsMoving = false;
                inputPacket << Packet::InputType::StopMoving;
            }

            objects.netManager.sendPacketUdp(inputPacket);
        }
    }
    elapsedTime = clock.restart().asSeconds();
}

void PlayGameState::handleMouseInput()
{
    if (!hasFocus && myPlayer != nullptr)
    {
        // Handle aiming with mouse
        sf::Vector2i mousePos = sf::Mouse::getPosition(objects.window);
        sf::Vector2f playerPos = myPlayer->getPos();
        sf::Vector2f viewMousePos = objects.window.mapPixelToCoords(mousePos, gameView);
        float angle = atan2(viewMousePos.y - playerPos.y, viewMousePos.x - playerPos.x);
        angle *= (180.0 / 3.14159265358979); // TODO: Make radian/degree converting functions
        //cout << "Angle: " << angle << endl;
        myPlayer->setVisualAngle(angle);
        currentAngle = angle;
    }
}

void PlayGameState::sendAngleInputPacket()
{
    // Update the server with your player's visual angle up to 5 times per second
    if (myPlayer != nullptr && angleTimer.getElapsedTime().asSeconds() >= 0.2 && lastSentAngle != currentAngle)
    {
        sf::Packet anglePacket;
        anglePacket << Packet::Input << Packet::InputType::ChangeVisualAngle << currentAngle;
        objects.netManager.sendPacketUdp(anglePacket);
        lastSentAngle = currentAngle;
        angleTimer.restart();
    }
}

void PlayGameState::processEntityPackets()
{
    while (objects.netManager.arePackets(Packet::EntityUpdate))
    {
        EID entId;
        sf::Packet& packet = objects.netManager.getPacket(Packet::EntityUpdate);
        while (packet >> entId)
            entList.updateEntity(entId, packet);
        objects.netManager.popPacket(Packet::EntityUpdate);
    }
}

void PlayGameState::processPlayerIdPackets()
{
    while (objects.netManager.arePackets(Packet::PlayerEntityId))
    {
        objects.netManager.getPacket(Packet::PlayerEntityId) >> myPlayerId;
        objects.netManager.popPacket(Packet::PlayerEntityId);
    }
    myPlayer = entList.find(myPlayerId);
}

void PlayGameState::takeScreenshot()
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
