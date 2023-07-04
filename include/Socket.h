#pragma once
//#ifndef SOCKET_H
//#define SOCKET_H

#include "AvaObj.h"
#include "definitions.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <system_error>
#include <string>
#include <Windows.h>
#include <vector>
#include <memory>
#include <bitset>
#include <cstddef>
#include <iostream>

#pragma comment(lib, "ws2_32.lib") //link against the Winsock library

class WSASession {
    public:
        WSASession();
        ~WSASession();
    protected:
        WSAData data;
};


class UDPSocket {
    protected:
        SOCKET sock;
        WSASession Session;

    public:
        UDPSocket();
        ~UDPSocket();

        uint32_t SendTo(sockaddr_in& remoteIP, const char* buffer, long unsigned int flags = 0);
        int RecvFrom(sockaddr_in remoteIP, char* buffer, uint32_t &numBytesReceived, LPWSAOVERLAPPED overlapped, long unsigned int flags = 0, LPWSAOVERLAPPED_COMPLETION_ROUTINE callback = NULL);
        void Bind(sockaddr_in &localIP, unsigned short port); //for associating a local address with the socket
        void Connect(sockaddr_in& remoteIP); //for connecting the socket to the remote address
};

//#endif // SOCKET_H
