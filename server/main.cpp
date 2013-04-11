#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <SFML/Network.hpp>
//#include "../shared/packet.h"

/*
TODO:
Create classes for stuff that this server will need to do.
Split those up into separate source code files.
Make it so the IP address is NOT sent with the packet, for security reasons. You can determine which client sent the packet with the socket.
    UDP is much more simple: You directly get the address of who sent each packet that was received.
    For sending them, you must specify an IP to send to every time.
Make it so this server accepts different packet types.
    Will need to make an enum with the types, and probably a switch statement to accept them.
    On unsupported or not yet implemented types, the packets can simply be ignored.
    Program some of the packet types, like chat messages and player position updates.
*/

using namespace std;

const int msgPort = 55001;
void msgSvr();
void welcome();

int main()
{
    welcome();
    msgSvr();
    //thread t1(msgSvr);
    //t1.join();
/*
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
}

void welcome()
{
    //Print Welcome Message Here
    cout << "ZombieSurvivalGame Server v0.1.0.0 Dev\n\n";

    //Print server IP Address
    //sf::IpAddress severAddressLAN = sf::IpAddress::getLocalAddress();
    //cout << "The server's LAN IP Address is: " << severAddressLAN << endl;
    //sf::IpAddress serverAddressWAN = sf::IpAddress::getPublicAddress();
    //cout << "The server's WAN IP Address is: " << serverAddressWAN << endl;
}

void msgSvr()
{
    int type = 1;
    string msg;

    // Create a socket to listen to new connections
    sf::TcpListener listener;
    listener.listen(msgPort);

    // Create a list to store the future clients
    list<sf::TcpSocket*> clients;

    // Create a selector
    sf::SocketSelector selector;

    // Add the listener to the selector
    selector.add(listener);

    // Endless loop that waits for new connections
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

                    cout << "Client connected, here is the current list:\n";
                    for (auto i = clients.begin(); i != clients.end(); ++i) // loop through the connected clients
                    {
                        sf::TcpSocket& client = **i;
                        cout << client.getRemoteAddress() << endl;
                    }
                }
            }
            else
            {
                // The listener socket is not ready, test all other sockets (the clients)
                for (auto it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client))
                    {
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done)
                        {
                            packet >> type;
                            switch (type)
                            {
                                case 1:{
                                    packet >> msg;
                                    cout << "Message: " << msg << endl;
                                    packet.clear();
                                    packet << msg;
                                    for (auto i = clients.begin(); i != clients.end(); ++i) // loop through the connected clients
                                    {
                                        sf::TcpSocket& clientToSend = **i;
                                        if (i != it) // don't send the message back to the client who sent it!
                                            clientToSend.send(packet); // send the message to the other client
                                    }
                                    break;}
                                default:
                                    cout << "Error: Unrecognized packet type?\n";
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
