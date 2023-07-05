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
    public:
        UDPSocket();
        ~UDPSocket();

        long unsigned int SendTo(sockaddr_in& remoteIP, const char* buffer, long unsigned int flags = 0);
        int RecvFrom(sockaddr_in remoteIP, char* buffer, long unsigned int &numBytesReceived, LPWSAOVERLAPPED overlapped, long unsigned int flags = 0, LPWSAOVERLAPPED_COMPLETION_ROUTINE callback = NULL);
        void Bind(sockaddr_in &localIP, unsigned short port); //for associating a local address with the socket
        void Connect(sockaddr_in& remoteIP); //for connecting the socket to the remote address
};
