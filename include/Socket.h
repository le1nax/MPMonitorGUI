#ifndef SOCKET_H
#define SOCKET_H

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <string>
#include <iostream>
#include <algorithm> //for std::reverse in a code example, to be deleted later on

#pragma comment(lib, "ws2_32.lib") //link against the Winsock library

class WSASession {
    public:
        WSASession();
        ~WSASession();
    private:
        WSAData data;
};


class UDPSocket {
    private:
        SOCKET sock;
    public:
        UDPSocket();
        ~UDPSocket();

        void SendTo(const std::string& address, unsigned short port, const char* buffer, int len, int flags = 0);
        void SendTo(sockaddr_in& address, const char* buffer, int len, int flags = 0);
        sockaddr_in RecvFrom(char* buffer, int len, int flags = 0);
        void Bind(unsigned short port);
};

#endif // SOCKET_H
