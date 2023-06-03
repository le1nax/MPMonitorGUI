#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include "Socket.h"

//


int main() //exemplary code for a client
{
    try
    {
        WSASession Session; //initialize the windows sockets api session
        UDPSocket Socket; //create a udp socket
        
        std::string data = "hello world"; //define an example string to send
        char buffer[100]; //create a buffer to hold received data

        Socket.SendTo("127.0.0.1", 100, data.c_str(), data.size()); // Send data to IP address and port 100
        Socket.RecvFrom(buffer, 100); // Receive data of length 100 into the buffer
        std::cout << buffer; // Output the received data
    }
    catch (std::exception &ex) //catch any occurring system errors
    {
        std::cout << ex.what();  //print error message
    }
    char c;
    std::cin >> c; // Wait for user input before exiting
}

#endif // SOCKET_CLIENT_H
