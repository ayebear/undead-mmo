// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "game.h"
#include "../shared/packet.h"

const std::string version = "Project: Brains v0.0.0.16 Dev";

Game::Game()
{

    // Load files and stuff
    if (!playerTex.loadFromFile("data/images/characters/character.png"))
        exit(Errors::Graphics);
    playerTex.setSmooth(true);
    if (!zombieTex.loadFromFile("data/images/characters/zombie.png"))
        exit(Errors::Graphics);
    zombieTex.setSmooth(true);
    // TODO: Will need to send a request to the server (during or after the log-in process)
    // which will create a new entity on the server first, which gets a unique global ID,
    // and then that gets sent right back to the player who just logged in, and then
    // is allocated on the client.
    // Normally this would be called when a packet is received of type "new entity". And the ID would be received from the server.
    entList.Add(Entity::Player, 1);

    // Add quite a few local test zombies for now
    for (int x = 2; x < 1000; x++)
    {
        auto* zombie = entList.Add(Entity::Zombie, x);
        zombie->SetTexture(zombieTex);
        zombie->SetPos(sf::Vector2f(rand() % windowWidth, rand() % windowHeight));
        zombie->SetAngle(rand() % 360);
    }

    myPlayer = entList.Find(1);
    myPlayer->SetTexture(playerTex);

    // Create the window in fullscreen at max resolution
    //vidMode = sf::VideoMode::getDesktopMode();
    //window.create(vidMode, version, sf::Style::Fullscreen);

    // Create a normal window for now
    vidMode = sf::VideoMode(windowWidth, windowHeight);
    window.create(vidMode, version);


    // Set frame limits and vsync
    //window.setFramerateLimit(10);
    window.setVerticalSyncEnabled(true);

    chat.SetPosition(0, windowHeight - 182); //   Windowed Mode
    //chat.SetPosition(0, vidMode.height - 182);  // Full- screen

    playing = true;
    paused = false;
}

// TODO: Handle networking code! Receiving and sending with both TCP and UDP!
// Also integrate that with the chat class.
void Game::Start() // this will need to manage a second thread for the networking code
{
    Menu menu(window, vidMode);
    int choice = menu.processChoice(window);

    if(choice == 1)
    {

        socket.setBlocking(false);
        chat.PrintMessage("Warning: Currently not connected to a server! Please type '/help connect' for more info.", sf::Color::Yellow);

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
    //Change to whatever option quit is after we add more menu choices
    else if(choice == 2 || choice == 3)
    {
        return;
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
            chat.PrintMessage(message);
        }
        /*
        This will simply pass the type received into the entity list object which will choose the proper class
            to use to allocate a new object.

        We can then stream in the rest of the packet data into the newly created entity object.
        */
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
                    default:
                        break;
                }
                if (chat.GetInput())
                {
					switch (event.key.code)
		            {
		                case sf::Keyboard::BackSpace:
		                    chat.Backspace();
		                    break;
		                case sf::Keyboard::Delete:
		                	chat.Delete();
		                	break;
		                case sf::Keyboard::Up:
		                    chat.MessageHistoryUp();
		                    break;
		                case sf::Keyboard::Down:
		                    chat.MessageHistoryDown();
		                    break;
		                case sf::Keyboard::Left:
		                	chat.MoveCursorLeft();
		                	break;
		                case sf::Keyboard::Right:
		                	chat.MoveCursorRight();
		                	break;
                        case sf::Keyboard::Home:
                            chat.Home();
                            break;
                        case sf::Keyboard::End:
                            chat.End();
                            break;
		                default:
		                    break;
		            }
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
            case sf::Event::Resized:
            {
                //Minimum window size
                sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
                // Minimum size
                if(size.x < 800)
                    size.x = 800;
                if(size.y < 600)
                    size.y = 600;
                window.setSize(static_cast<sf::Vector2u>(size));
                //Reset the view of the window
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                chat.SetPosition(0, event.size.height - 182);
                break;
            }
            default:
                break;
        }
    }
}

void Game::ProcessInput()
{
    if (!paused && !chat.GetInput())
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

    chat.Update();
}

void Game::Display()
{
    window.clear();

    // Draws all of the entities
    window.draw(entList);

    // Draws the chat
    window.draw(chat);

    window.display();
}
