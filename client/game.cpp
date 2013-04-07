#include "game.h"

Game::Game()
{
    // Load files and stuff
    playerTex.loadFromFile("data/images/characters/character.png");
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

    playing = true;
}

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
                        playing = false;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }
}

void Game::ProcessInput()
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

void Game::Update()
{
    // All of the processing code will be run from here.
}

void Game::Display() // TODO: Use a rendering class instead
{
    window.clear();

    window.draw(player.sprite);

    window.display();
}
