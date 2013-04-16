// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
TODO:
Also handle UDP stuff instead of just TCP. Do this on the same thread.
Make it so the IP address is NOT sent with the packet, for security reasons. You can determine which client sent the packet with the socket.
    UDP is much more simple: You directly get the address of who sent each packet that was received.
    For sending them, you must specify an IP to send to every time.
Make it so this server accepts different packet types.
    Will need to make an enum with the types, and probably a switch statement to accept them.
    On unsupported or not yet implemented types, the packets can simply be ignored.
    Program some of the packet types, like chat messages and player position updates.
*/

#include "server.h"

using namespace std;

Server::Server()
{
}

void Server::Start()
{
    PrintWelcomeMsg();
    MainLoop();
}

void Server::PrintWelcomeMsg()
{
    //Print Welcome Message Here
    cout << "ZombieSurvivalGame Server v0.1.1.0 Dev\n\n";

    //Print server IP Address
    sf::IpAddress severAddressLAN = sf::IpAddress::getLocalAddress();
    cout << "The server's LAN IP Address is: " << severAddressLAN << endl;
    sf::IpAddress serverAddressWAN = sf::IpAddress::getPublicAddress();
    cout << "The server's WAN IP Address is: " << serverAddressWAN << endl;
}

void Server::SendToClients(sf::Packet& packet, int exclude)
{
    for (int i = 0; i != (int)clients.size(); ++i) // loop through the connected clients
    {
        if (i != exclude) // don't send the packet back to the client who sent it!
            clients[i]->send(packet); // send the packet to the other clients
    }
}

void Server::MainLoop()
{
    // Have the listener listen on the port
    listener.listen(serverPort);

    // Add the listener to the selector
    selector.add(listener);

    bool running = true;
    while (running)
    {
        // Make the selector wait for data on any socket
        if (selector.wait())
        {
            // Test the listener
            if (selector.isReady(listener))
            {
                // The listener is ready: there is a pending connection
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done)
                {
                    // Add the new client to the clients list
                    clients.push_back(client);

                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    selector.add(*client);

                    cout << "Client " << client->getRemoteAddress() << " connected, here is the current list:\n";
                    for (auto& c: clients) // loop through the connected clients
                        cout << c->getRemoteAddress() << endl;
                }
            }
            else
            {
                // The listener socket is not ready, test all other sockets (the clients)
                for (uint i = 0; i != clients.size(); ++i)
                {
                    auto& client = *clients[i];
                    if (selector.isReady(client))
                    {
                        // The client has sent some data, we can receive it
                        int type = 1;
                        string msg;
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done)
                        {
                            packet >> type;
                            switch (type)
                            {
                                case Packet::ChatMessage:{
                                    packet >> msg;
                                    cout << "Message: " << msg << endl;
                                    SendToClients(packet, i);
                                    break;}
                                case Packet::PlayerUpdate:{
                                    float x, y;
                                    packet >> x >> y;
                                    // x and y need to be stored in the player object in the entity list
                                    // in the future this will be input and time instead of direct coordinates
                                    // also, when sending the data to the clients, it needs the entity ID with it of course
                                    SendToClients(packet, i);
                                    break;}
                                default:
                                    cout << "Error: Unrecognized packet type? Type was: " << type << endl;
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
Misc code section
*/

/*
    welcome();
    msgSvr();
    //thread t1(msgSvr);
    //t1.join();

     // ----- The server -----

    // Create a listener to wait for incoming connections on port 55001
    sf::TcpListener listener;
    listener.listen(55001);
    cout << "Listening on port 55001...";

    // Wait for a connection
    sf::TcpSocket socket;
    listener.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;

    // Receive a message from the client
    sf::Packet packet;
    //char buffer[1024];
    //std::size_t received = 0;
    //socket.receive(buffer, sizeof(buffer), received);
    socket.receive(packet);
    int type;
    string message;
    packet >> type >> message;
    std::cout << "Type: " << type << ", The client said: " << message << std::endl;

    // Send an answer
    message = "Welcome, client";
    packet.clear();
    packet << message;
    socket.send(packet);*/

/*
void msgSvr)
{
    // Create a socket for listening to incoming connections
    sf::SocketTCP dataListener;
    if (!dataListener.Listen(msgPort))
        return;
    std::cout << "Listening to port " << msgPort << ", waiting for connections..." << std::endl;

    // Create a selector for handling several sockets (the listener + the socket associated to each client)
    sf::SocketSelector Selector;

    // Add the listener
    Selector.Add(dataListener);

    // Loop while... we close the program :)
    while (true)
    {
        // Get the sockets ready for reading
        unsigned int NbSockets = Selector.Wait();

        // We can read from each returned socket
        for (unsigned int i = 0; i < NbSockets; ++i)
        {
            // Get the current socket
            sf::SocketTCP dataSocket = Selector.GetSocketReady(i);

            if (dataSocket == dataListener)
            {
                // If the listening socket is ready, it means that we can accept a new connection
                sf::IpAddress Address;
                sf::SocketTCP Client;
                dataListener.Accept(Client, &Address);
                std::cout << "Client connected ! (" << Address << ")" << std::endl;

                // Add it to the selector
                Selector.Add(Client);
            }
            else
            {
                // Else, it is a client socket so we can read the data he sent
                sf::Packet msgPacket;
                if (dataSocket.Receive(msgPacket) == sf::Socket::Done)
                {
                    // Extract the message and display it
                    std::string alias = "null", clientIP = "null", textMessage = "null";
                    msgPacket >> alias >> clientIP >> textMessage;
                    std::cout << alias << " @ " << clientIP << " says: " << textMessage << std::endl;
                    //messages.push_back(textMessage);
                }
                else
                {
                    // Error : we'd better remove the socket from the selector
                    Selector.Remove(dataSocket);
                }
            }
        }
    }

}
*/
