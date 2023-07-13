#pragma once

#include "AvaObj.h"
#include "definitions.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <Windows.h>
#include <bitset>
#include <cstddef>

#pragma comment(lib, "ws2_32.lib") //link against the Winsock library

/// @brief The WSASession class is used to start and close the WSA (Winsock API) session.
class WSASession {
    public:
        WSASession();
        ~WSASession();
    protected:
        WSAData data;
};


/// @brief The UDPSocket class is the UDP client and server base class implementing the basic functions for sending, receiving, binding, and connecting between server and client.
class UDPSocket {
    protected:
        SOCKET sock;
    public:
        UDPSocket();
        ~UDPSocket();
        SOCKET getSocket();
        long unsigned int SendTo(sockaddr_in& remoteIP, char* buffer, size_t buffersize, long unsigned int flags = 0);
        int RecvFrom(sockaddr_in remoteIP, char* buffer, size_t buffersize, long unsigned int &numBytesReceived, LPWSAOVERLAPPED overlapped, long unsigned int flags = 0, LPWSAOVERLAPPED_COMPLETION_ROUTINE callback = NULL);
        int alternativeRecFrom();
        void Bind(sockaddr_in &localIP, unsigned short port); //for associating a local address with the socket, server side
        void Connect(sockaddr_in& remoteIP); //for connecting the socket to the remote address
};
