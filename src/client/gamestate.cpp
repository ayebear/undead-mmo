// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "gamestate.h"
#include <string>
#include "packet.h"
#include "tile.h"
#include "takescreenshot.h"
#include "paths.h"

GameState::GameState(GameObjects& gameObjects): CommonState(gameObjects)
{
    loadHotkeys();

    // Load textures
    Tile::loadTextures();
    Entity::loadTextures();

    myPlayer = nullptr;
    myPlayerId = 0;
    playerIsMoving = false;
    gameView.setSize(objects.windowSize.x, objects.windowSize.y);
    theHud.setUp(objects);

    playerInput.x = 0;
    playerInput.y = 0;
    currentAngle = 0;
    lastSentAngle = 0;

    playing = true;
    hasFocus = true;
}

GameState::~GameState()
{
}

void GameState::onPush()
{
    mouseMoved = true;
    theHud.chat.clear();
    theHud.chat.setUsername(objects.netManager.getUsername());
    inventoryKeyReleased = true;
}

void GameState::onPop()
{
    objects.netManager.logOut();
    myPlayer = nullptr;
    myPlayerId = 0;
    entList.clear();
}

void GameState::onStart()
{
    objects.music.start("Game");
}

void GameState::handleEvents()
{
    // Exit the game if the server disconnects
    if (!objects.netManager.isConnected())
    {
        stateEvent.popState();
        return;
    }

    sf::Event event;
    while (objects.window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                stateEvent.exitGame();
                break;

            case sf::Event::KeyPressed:
                handleKeyPressed(event.key.code);
                break;

            case sf::Event::KeyReleased:
                handleKeyReleased(event.key.code);
                break;

            case sf::Event::MouseButtonPressed:
                theHud.chat.handleMouseClicked(event, objects.window);
                theHud.inventory.handleMouseClicked(event);
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
                theHud.inventory.handleMouseMoved(event);
                break;

            case sf::Event::LostFocus:
                hasFocus = false;
                break;

            case sf::Event::GainedFocus:
                hasFocus = true;
                break;

            case sf::Event::Resized:
                handleWindowResized();
                break;

            default:
                break;
        }
    }
    handleInput();
}

void GameState::update()
{
    objects.music.update();

    if (!tileMap.isReady())
        processMapDataPackets();

    processEntityPackets();

    if (myPlayer == nullptr)
        processOnLogInPackets();

    entList.update(elapsedTime);

    theHud.inventory.processPackets(objects.netManager);

    updateGameView();

    // TODO: Figure out the best way to do this
    if (mouseMoved || playerIsMoving)
    {
        handleMouseInput();
        mouseMoved = false;
    }
    sendAngleInputPacket();

    theHud.update();
}

void GameState::draw()
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

void GameState::updateGameView()
{
    // Update the game view center position with the player's current position
    if (myPlayer != nullptr)
        gameView.setCenter(myPlayer->getPos());

    // Do not show anything past the boundaries of the map
    if (tileMap.isReady())
    {
        sf::Vector2f viewSize(gameView.getSize());
        sf::Vector2f viewCenter(gameView.getCenter());
        if (viewCenter.x - (viewSize.x / 2) < 0)
        {
            gameView.setCenter(viewSize.x / 2, viewCenter.y);
            viewCenter = gameView.getCenter();
        }
        if (viewCenter.y - (viewSize.y / 2) < 0)
        {
            gameView.setCenter(viewCenter.x, viewSize.y / 2);
            viewCenter = gameView.getCenter();
        }
        if (viewCenter.x + (viewSize.x / 2) >= tileMap.getWidthPx())
        {
            gameView.setCenter(tileMap.getWidthPx() - viewSize.x / 2, viewCenter.y);
            viewCenter = gameView.getCenter();
        }
        if (viewCenter.y + (viewSize.y / 2) >= tileMap.getHeightPx())
        {
            gameView.setCenter(viewCenter.x, tileMap.getHeightPx() - viewSize.y  / 2);
            viewCenter = gameView.getCenter();
        }
    }
}

void GameState::handleKeyPressed(sf::Keyboard::Key keyCode)
{
    // TODO: Shrink this mess into 1 line of code with a proper generic event handler...
    if (theHud.chat.getInput())
    {
        if (keyCode == hotkeys[HideChat])
            theHud.chat.setInput(false);
        else if (keyCode == sf::Keyboard::Return)
        {
            theHud.chat.parseMessage();
            theHud.chat.toggleInput();
        }
    }
    else
    {
        if (keyCode == sf::Keyboard::Escape)
            stateEvent.popState();
        else if (keyCode == hotkeys[ShowChat])
            theHud.chat.toggleInput();
        else if (keyCode == hotkeys[ToggleInventory] || keyCode == hotkeys[ToggleInventoryAlt])
        {
            if (inventoryKeyReleased)
            {
                inventoryTimer.restart();
                theHud.inventory.toggleInventory();
            }
            inventoryKeyReleased = false;
        }
        else if (keyCode == hotkeys[AddSlot])
            theHud.inventory.addSlots(1);
        else if (keyCode == hotkeys[RemoveSlot])
            theHud.inventory.addSlots(-1);
        else if (keyCode == hotkeys[TakeScreenshot])
            takeScreenshot(objects.window, Paths::screenshotsDir);
        else if (keyCode == hotkeys[Reload])
            objects.sound.play("reload");
        else if (keyCode == hotkeys[Shoot])
            objects.sound.play("pistol");
    }
    theHud.chat.processInput(keyCode);
}

void GameState::handleKeyReleased(sf::Keyboard::Key keyCode)
{
    if (!theHud.chat.getInput())
    {
        if (keyCode == hotkeys[ToggleInventory] || keyCode == hotkeys[ToggleInventoryAlt])
        {
            if (inventoryTimer.getElapsedTime().asMilliseconds() >= 500 && theHud.inventory.getVisibility())
                theHud.inventory.toggleInventory();
            inventoryKeyReleased = true;
        }
    }
}

void GameState::handleInput()
{
    if (hasFocus && !theHud.chat.getInput())
    {
        // This is horrible code I wrote, we should make it better
        oldPlayerInput = playerInput;
        playerInput.x = 0;
        playerInput.y = 0;
        if (sf::Keyboard::isKeyPressed(hotkeys[MoveUp]) || sf::Keyboard::isKeyPressed(hotkeys[MoveUpAlt]))
            playerInput.y--; // 90
        if (sf::Keyboard::isKeyPressed(hotkeys[MoveDown]) || sf::Keyboard::isKeyPressed(hotkeys[MoveDownAlt]))
            playerInput.y++; // 270 (or -90)
        if (sf::Keyboard::isKeyPressed(hotkeys[MoveLeft]) || sf::Keyboard::isKeyPressed(hotkeys[MoveLeftAlt]))
            playerInput.x--; // 180
        if (sf::Keyboard::isKeyPressed(hotkeys[MoveRight]) || sf::Keyboard::isKeyPressed(hotkeys[MoveRightAlt]))
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

void GameState::handleMouseInput()
{
    if (hasFocus && myPlayer != nullptr)
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

void GameState::sendAngleInputPacket()
{
    // Update the server with your player's visual angle up to 5 times per second
    if (myPlayer != nullptr && angleTimer.getElapsedTime().asSeconds() >= 0.1 && lastSentAngle != currentAngle)
    {
        sf::Packet anglePacket;
        anglePacket << Packet::Input << Packet::InputType::ChangeVisualAngle << currentAngle;
        objects.netManager.sendPacketUdp(anglePacket);
        lastSentAngle = currentAngle;
        angleTimer.restart();
    }
}

void GameState::processEntityPackets()
{
    while (objects.netManager.arePackets(Packet::EntityUpdate))
    {
        EID entId;
        sf::Packet& packet = objects.netManager.getPacket(Packet::EntityUpdate);
        int count = 0;
        while (packet >> entId)
        {
            entList.updateEntity(entId, packet);
            ++count;
        }
        if (count >= 10)
            cout << "Updated " << count << " entities from server.\n";
        objects.netManager.popPacket(Packet::EntityUpdate);
    }
}

void GameState::processOnLogInPackets()
{
    while (objects.netManager.arePackets(Packet::OnSuccessfulLogIn))
    {
        objects.netManager.getPacket(Packet::OnSuccessfulLogIn) >> myPlayerId;
        objects.netManager.popPacket(Packet::OnSuccessfulLogIn);
    }
    myPlayer = entList.find(myPlayerId);
}

void GameState::processMapDataPackets()
{
    while (objects.netManager.arePackets(Packet::MapData))
    {
        tileMap.loadFromPacket(objects.netManager.getPacket(Packet::MapData));
        Entity::setMapSize(tileMap.getWidthPx(), tileMap.getHeightPx());
        objects.netManager.popPacket(Packet::MapData);
        cout << "Received map from server. Size: " << tileMap.getWidth() << " by " << tileMap.getHeight() << ".\n";
    }
}

void GameState::handleWindowResized()
{
    sf::Vector2f windowSize;
    windowSize.x = objects.window.getSize().x;
    windowSize.y = objects.window.getSize().y;
    // Reset the view of the window
    gameView.setSize(windowSize);
    // objects.window.setView(gameView);
    viewDimensions = objects.window.getView().getSize();

    theHud.updateView(gameView);
}

void GameState::loadHotkeys()
{
    objects.config.useSection("Hotkeys");
    const string keyNames[] = {
        "moveUp",
        "moveDown",
        "moveLeft",
        "moveRight",
        "moveUpAlt",
        "moveDownAlt",
        "moveLeftAlt",
        "moveRightAlt",
        "toggleInventory",
        "toggleInventoryAlt",
        "showChat",
        "hideChat",
        "takeScreenshot",
        "addSlot",
        "removeSlot",
        "reload",
        "shoot"
        };
    for (const string& key: keyNames)
        hotkeys.addKey(objects.config(key).toString());
    objects.config.useSection();

    /*
    auto hotkeysSection = GameObjects::defaultOptions.find("Hotkeys");
    if (hotkeysSection != GameObjects::defaultOptions.end())
    {
        // The map is ordered so they are not loaded properly...
        for (auto& key: hotkeysSection->second)
            hotkeys.addKey(objects.config(key.first].asString());
    }
    */
}
