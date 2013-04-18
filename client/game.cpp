// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "game.h"
#include "../shared/packet.h"

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
    socket.connect("127.0.0.1", 55001);
    socket.setBlocking(false);

    sf::Clock clock;
    while (playing && window.isOpen())
    {
        ReceiveData();
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

void Game::ReceiveData()
{
    sf::Packet packet;
    if (socket.receive(packet) == sf::Socket::Done)
    {
        int type;
        packet >> type;
        if (type == Packet::ChatMessage)
        {
            string message;
            packet >> message;
            chat.AddMessage(message);
        }
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
                        chat.SetInput(false);
                        break;
                    case sf::Keyboard::Return:
                        if (chat.GetInput())
                            chat.ParseMessage(socket);
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
            case sf::Event::LostFocus:
                paused = true;
                break;
            case sf::Event::GainedFocus:
                paused = false;
                break;
            default:
                break;
        }
    }
}

void Game::ProcessInput()
{
    if (!paused && !chat.GetInput())
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

void Game::Display()
{
    window.clear();

    window.draw(player);

    window.draw(chat);

    window.display();
}
