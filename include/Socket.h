#pragma once
//#ifndef SOCKET_H
//#define SOCKET_H

#include "AvaObj.h"
#include "definitions.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <Windows.h>
#include <bitset>
#include <cstddef>

//#pragma comment(lib, "ws2_32.lib")

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

        uint32_t SendTo(const std::string& address_string, unsigned short port, const char* buffer, LPWSAOVERLAPPED overlapped, long unsigned int flags = 0);
        uint32_t SendTo(sockaddr_in& remoteIP, const char* buffer, LPWSAOVERLAPPED overlapped, long unsigned int flags = 0);
        int RecvFrom(sockaddr_in remoteIP, char* buffer, uint32_t &numBytesReceived, LPWSAOVERLAPPED overlapped, long unsigned int flags = 0, LPWSAOVERLAPPED_COMPLETION_ROUTINE callback = NULL);
        void Bind(sockaddr_in &localIP, unsigned short port); //for associating a local address with the socket
        void Connect(sockaddr_in& remoteIP); //for connecting the socket to the remote address
};

//#endif // SOCKET_H
