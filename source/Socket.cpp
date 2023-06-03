#include "../include/Socket.h"
#include <iostream>


WSASession::WSASession() {
    //calling Startup fct with Winsock version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        throw std::system_error(WSAGetLastError(), std::system_category(), "WSAStartup Failed");
    }
}


WSASession::~WSASession() {
    WSACleanup();
}




UDPSocket::UDPSocket() {
    //AF_INET = IPv4; SOCK_DGRAM = Byte stream for UDP; IPPROTO_UDP = UDP Protocol
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET)
        throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket");
}


UDPSocket::~UDPSocket() {
    closesocket(sock);
}


void UDPSocket::SendTo(const std::string& address, unsigned short port, const char* buffer, int len, int flags = 0) {
    //send data provided in buffer to receiver address and port
    //len is the length of the data in byte and flags provides additional sending options

    sockaddr_in add; //receiver address
    add.sin_family = AF_INET; //ipv4

    //IP address passed as string and converted to 32-bit binary format via inet_addr()
    add.sin_addr.s_addr = inet_addr(address.c_str());

////////////////////inet_addr() is deprecated apparently, can only do ipv4. next line might be how you'd do it today///////////////////////////////////
////////////////////WSAStringToAddressA(const_cast<char*>(address.c_str()), AF_INET, NULL, (struct sockaddr*)&add, &len);//////////////////////////////
    
    //port number passed as unsigned short, converted from host byte order to network byte order via htons()
    add.sin_port = htons(port);
    int ret = sendto(sock, buffer, len, flags, reinterpret_cast<SOCKADDR*>(&add), sizeof(add));
    if (ret < 0)
        throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
}


void UDPSocket::SendTo(sockaddr_in& address, const char* buffer, int len, int flags = 0) {
    //send data provided in buffer to receiver address
    //len is the length of the data in byte and flags provides additional sending options

    int ret = sendto(sock, buffer, len, flags, reinterpret_cast<SOCKADDR*>(&address), sizeof(address));
    if (ret < 0)
        throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
}


sockaddr_in UDPSocket::RecvFrom(char* buffer, int len, int flags = 0) {
    sockaddr_in from; //sender's address information
    int size = sizeof(from);

    //receive data from sock and store it in buffer
    int ret = recvfrom(sock, buffer, len, flags, reinterpret_cast<SOCKADDR*>(&from), &size);
    if (ret < 0)
        throw std::system_error(WSAGetLastError(), std::system_category(), "recvfrom failed");

    buffer[ret] = 0; //make the buffer zero terminated, marking end of string
    return from;
}


void UDPSocket::Bind(unsigned short port) {
    sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = htonl(INADDR_ANY); //allows to bind to any available local network interface
    add.sin_port = htons(port); //htons converts port number from host byte order to network byte order

    int ret = bind(sock, reinterpret_cast<SOCKADDR*>(&add), sizeof(add)); //bind sock to add (cast to a sockaddr pointer) and port
    if (ret < 0)
        throw std::system_error(WSAGetLastError(), std::system_category(), "Bind failed");
}
