#ifndef SOCKET_H
#define SOCKET_H

#include "AvaObj.h"
#include "definitions.h"
#include <Winsock2.h>
#include <WS2tcpip.h>
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
        WSASocket sock;
        WSASession Session;

        uint32_t numBytesSent = 0; //will in the end contain the number of bytes sent
        uint32_t numBytesReceived = 0; //will in the end contain the number of bytes received


    public:
        UDPSocket();
        ~UDPSocket();

        void SendTo(const std::string& address_string, unsigned short port, const char* buffer, LPWSAOVERLAPPED overlapped, int flags = 0);
        void SendTo(sockaddr_in& remoteIP, const char* buffer, LPWSAOVERLAPPED overlapped, int flags = 0);
        int RecvFrom(sockaddr_in remoteIP, char* buffer, LPWSAOVERLAPPED overlapped, int flags = 0, LPWSAOVERLAPPED_COMPLETION_ROUTINE callback = NULL);
        void Bind(sockaddr_in &localIP, unsigned short port); //for associating a local address with the socket
        void Connect(sockaddr_in& remoteIP); //for connecting the socket to the remote address
};

#endif // SOCKET_H
