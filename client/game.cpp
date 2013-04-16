// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "game.h"

Game::Game()
{
    // Load files and stuff
    if (!playerTex.loadFromFile("data/images/characters/character.png"))
        exit(2);
    player.SetTexture(playerTex);

    // Create the window in fullscreen at max resolution
    //vidMode = sf::VideoMode::GetMode(0);
    //window.Create(vidMode, version, sf::Style::Fullscreen);

    // Create a normal window for now
    vidMode = sf::VideoMode(windowWidth, windowHeight);
    window.create(vidMode, version, sf::Style::Close);

    // Set frame limits and vsync
    //window.SetFramerateLimit(5);
    window.setVerticalSyncEnabled(true);

    chat.SetPosition(0, windowHeight - 182);

    playing = true;
}

// TODO: Handle networking code! Receiving and sending with both TCP and UDP!
// Also integrate that with the chat class.
void Game::Start() // this will need to manage a second thread for the networking code
{
    sf::Clock clock;
    while (playing && window.isOpen())
    {
        GetInput();
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

void Game::GetInput()
{
    // Get the current mouse and keyboard input
    //const sf::Input& input = window.GetInput();

    // Get mouse coordinates
    //mousePos = window.ConvertCoords(input.GetMouseX(), input.GetMouseY());
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
                        chat.SetInput(false);
                        break;
                    case sf::Keyboard::Return:
                        if (chat.GetInput())
                            chat.SendMessage();
                        chat.ToggleInput();
                        break;
                    case sf::Keyboard::BackSpace:
                        chat.RemoveChar();
                        break;
                    default:
                        break;
                }
                break;
            case sf::Event::TextEntered:
                if (chat.GetInput() && event.text.unicode >= 32 && event.text.unicode <= 126)
                    chat.AddChar(static_cast<char>(event.text.unicode));
                break;
            default:
                break;
        }
    }
}

// TODO: Disable most game input when chat is enabled
void Game::ProcessInput()
{
    if (!chat.GetInput())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            player.MoveUp(elapsedTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            player.MoveDown(elapsedTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            player.MoveLeft(elapsedTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            player.MoveRight(elapsedTime);
    }
}

void Game::Update()
{
    // All of the processing code will be run from here.

    chat.Update();
}

void Game::Display() // TODO: Use a rendering class instead
{
    window.clear();

    window.draw(player);

    window.draw(chat);

    window.display();
}
