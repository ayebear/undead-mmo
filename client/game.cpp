#include "game.h"

Game::Game()
{
    // Load files and stuff
    playerImg.LoadFromFile("data/images/characters/character.png");
    player.SetImage(playerImg);

    // Create the window in fullscreen at max resolution
    //vidMode = sf::VideoMode::GetMode(0);
    //window.Create(vidMode, version, sf::Style::Fullscreen);

    // Create a normal window for now
    vidMode = sf::VideoMode(windowWidth, windowHeight);
    window.Create(vidMode, version, sf::Style::Close);

    // Set frame limits and vsync
    //window.SetFramerateLimit(5);
    window.UseVerticalSync(true);

    playing = true;
}

void Game::Start() // this will need to manage a second thread for the networking code
{
    while (playing && window.IsOpened())
    {
        // Do we REALLY need a message pump system? These functions will do stuff when user input is detected.
        // All a message pump will do is delay that, and seems like an unnecessary hoop to jump through.
        // I think those are only required when dealing with the Windows API, SFML already does that for us. ^^
        GetInput();
        ProcessEvents();
        ProcessInput();

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

    // Get the elapsed time since last frame
    elapsedTime = window.GetFrameTime();

    // Get mouse coordinates
    //mousePos = window.ConvertCoords(input.GetMouseX(), input.GetMouseY());
}

void Game::ProcessEvents()
{
    sf::Event Event;
    while (window.GetEvent(Event))
    {
        switch (Event.Type)
        {
            case sf::Event::Closed:
                window.Close();
                break;
            case sf::Event::KeyPressed:
                switch (Event.Key.Code)
                {
                    case sf::Key::Escape:
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
    const sf::Input& input = window.GetInput();

    if (input.IsKeyDown(sf::Key::W) || input.IsKeyDown(sf::Key::Up))
        player.MoveUp(elapsedTime);
    if (input.IsKeyDown(sf::Key::S) || input.IsKeyDown(sf::Key::Down))
        player.MoveDown(elapsedTime);
    if (input.IsKeyDown(sf::Key::A) || input.IsKeyDown(sf::Key::Left))
        player.MoveLeft(elapsedTime);
    if (input.IsKeyDown(sf::Key::D) || input.IsKeyDown(sf::Key::Right))
        player.MoveRight(elapsedTime);
}

void Game::Update()
{
    // All of the processing code will be run from here.
}

void Game::Display() // TODO: Use a rendering class instead
{
    window.Clear();

    window.Draw(player.sprite);

    window.Display();
}
