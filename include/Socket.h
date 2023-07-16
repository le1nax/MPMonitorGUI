#pragma once

#include "AvaObj.h"
#include "definitions.h"

#include <system_error>
#include <Windows.h>
#include <bitset>
#include <cstddef>

#pragma comment(lib, "ws2_32.lib") //link against the Winsock library

///@brief WinsockAPI intitialisation class
///
///The WSASession class is used to start and close the WSA (Winsock API) session.
/// It is used to dynamically link the WSA Libs
class WSASession {
    public:
        /// @brief the constructor of the WSASession class
        WSASession();
        /// @brief the destructor of the WSASession class
        ~WSASession();
    protected:
        WSAData data;
};


///@brief Superclass for UDP Sockets
///
/// The UDPSocket class is the UDP client and server base class implementing the basic functions for sending, receiving, binding, and connecting between server and client.
class UDPSocket {
    protected:
        SOCKET sock;
    public:
        /// @brief the constructor of the UDPSocket class
        UDPSocket();
        
        /// @brief the destructor of the UDPSocket class
        ~UDPSocket();
        
        /// @brief a getter method to get the socket
        /// @return the membersocket
        SOCKET getSocket();
        
        /// @brief A method that calls the WSASendTo function for sending to the monitor
        /// @param remoteIP the remote IP address of the monitor
        /// @param buffer the buffer with the data to be sent
        /// @param buffersize the size of the buffer
        /// @param flags the flags to be set for the WSASendTo function
        /// @return the number of bytes sent
        long unsigned int SendTo(sockaddr_in& remoteIP, char* buffer, size_t buffersize, long unsigned int flags = 0);
        
        /// @brief A method that calls the WSARecvFrom function to receive data from the monitor
        /// @param remoteIP the remote IP address of the monitor
        /// @param buffer the buffer with the data to be sent
        /// @param buffersize the size of the buffer
        /// @param numBytesReceived the number of bytes received
        /// @param overlapped the WSAOverlapped structure for the WSARecvFrom function
        /// @param flags the flags to be set for the WSARecvFrom function
        /// @param callback the callback function to be passed to the WSARecvFrom function 
        /// @return the result of the WSARecvFrom function which says if the operation was successful
        int RecvFrom(sockaddr_in remoteIP, char* buffer, size_t buffersize, long unsigned int &numBytesReceived, LPWSAOVERLAPPED overlapped, long unsigned int flags = 0, LPWSAOVERLAPPED_COMPLETION_ROUTINE callback = NULL);
        
        /// @brief A method for associating a local address with the socket on the server side
        /// @param localIP the sockaddress_in structure with the local IP address, port and settings of the server address
        /// @param port is actually not used in this implementation because it's already part of the scokaddr_in structure localIP
        void Bind(sockaddr_in &localIP, unsigned short port); //for associating a local address with the socket, server side
        
        /// @brief A method for connecting the socket to the remote address. Careful: in UDP, this doesn't have the same implications as the connecting in TCP
        /// @param remoteIP the remote target address's sockaddr_in structure
        void Connect(sockaddr_in& remoteIP); //for connecting the socket to the remote address
};
