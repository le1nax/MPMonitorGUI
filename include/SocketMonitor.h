//#pragma once
// #ifndef SOCKET_MONITOR_H
// #define SOCKET_MONITOR_H

// #include "Socket.h"

// //


// int main() //exemplary code for a server //to be run first (i.e. before the Client)
// {
//     try
//     {
//         WSASession Session; //initialize the windows sockets api session
//         UDPSocket Socket; //create a udp socket

//         char buffer[100]; //create a buffer to hold received data

//         Socket.Bind(100); //bind the socket to a specific port (here: port 100)
//         while (true) //continuously receive and process incoming data
//         {
//             sockaddr_in add = Socket.RecvFrom(buffer, sizeof(buffer)); //receive data into the buffer and get the sender's address

//             std::string input(buffer); //convert the received data to string
//             std::reverse(std::begin(input), std::end(input)); //reverse string
//             Socket.SendTo(add, input.c_str(), input.size()); //send reversed string back to sender
//         }
//     }
//     catch (std::system_error& e) //catch any occurring system errors
//     {
//         std::cout << e.what(); //print error message
//     }
// }

// #endif // SOCKET_MONITOR_H
