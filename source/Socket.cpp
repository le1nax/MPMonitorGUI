#include "../include/Socket.h"
#include <iostream>
#include <cstdint>


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
        //std::cerr << "Failed to create socket. Error: " << WSAGetLastError() << std::endl;
}


UDPSocket::~UDPSocket() {
    closesocket(sock);
}


/// @todo error handling
void UDPSocket::SendTo(const std::string& address_string, unsigned short port, const char* buffer, LPWSAOVERLAPPED overlapped, int flags = 0) {
    //send data provided in buffer to receiver address and port

    sockaddr_in remoteIP; //receiver address
    remoteIP.sin_family = AF_INET; //ipv4
    remoteIP.sin_port = htons(port); //port number converted from host byte order to network byte order
    inet_pton(AF_INET, address_string.c_str(), &(remoteIP.sin_addr));

    int result = WSASendTo(
					sock,                                       		// SOCKET s
					reinterpret_cast<LPWSABUF>(&buffer),          		// LPWSABUF lpBuffers
					1,                                            		// DWORD dwBufferCount
					reinterpret_cast<LPDWORD>(&numBytesSent),    		// LPDWORD lpNumberOfBytesSent
					reinterpret_cast<DWORD>(&flags),            		// DWORD dwFlags
					reinterpret_cast<const sockaddr*>(&remoteIP),       // const sockaddr* lpTo
                    sizeof(remoteIP),         		                    // int iTolen
					reinterpret_cast<LPWSAOVERLAPPED>(&overlapped), 	// LPWSAOVERLAPPED lpOverlapped
					NULL                                       			// LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
				);
	if (result == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
        if (errorCode != WSA_IO_PENDING)
        {
            std::cerr << "Failed to start send operation: " << errorCode << std::endl;
            // Handle receive error
            // ...
        }
	}
}


/// @todo error handling
void UDPSocket::SendTo(sockaddr_in& remoteIP, const char* buffer, LPWSAOVERLAPPED overlapped, int flags = 0) {
    //send data provided in buffer to receiver address

    int result = WSASendTo(
					sock,                                       		// SOCKET s
					reinterpret_cast<LPWSABUF>(&buffer),          		// LPWSABUF lpBuffers
					1,                                            		// DWORD dwBufferCount
					reinterpret_cast<LPDWORD>(&numBytesSent),    		// LPDWORD lpNumberOfBytesSent
					reinterpret_cast<DWORD>(&flags),            		// DWORD dwFlags
					reinterpret_cast<const sockaddr*>(&remoteIP),       // const sockaddr* lpTo
                    sizeof(remoteIP),         		                    // int iTolen
					reinterpret_cast<LPWSAOVERLAPPED>(&overlapped), 	// LPWSAOVERLAPPED lpOverlapped
					NULL                                       			// LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
				);
	if (result == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
        if (errorCode != WSA_IO_PENDING)
        {
            std::cerr << "Failed to start send operation: " << errorCode << std::endl;
            // Handle receive error
            // ...
        }
	}
}


/// @todo error handling
/// @todo ByteArrayToFile and ReadData in case of overlap -- will that actually happen here, at all? Can we just leave it out?
int UDPSocket::RecvFrom(sockaddr_in remoteIP, char* buffer, LPWSAOVERLAPPED overlapped, int flags = 0, LPWSAOVERLAPPED_COMPLETION_ROUTINE callback = NULL) {
//receive data into buffer from remote sender address

    int remoteIPlen = sizeof(remoteIP);

    int result = WSARecvFrom(
					sock,                                       		// SOCKET s
					reinterpret_cast<LPWSABUF>(&buffer),          		// LPWSABUF lpBuffers
					1,                                            		// DWORD dwBufferCount
					reinterpret_cast<LPDWORD>(&numBytesReceived),       // LPDWORD lpNumberOfBytesRecvd
					reinterpret_cast<LPDWORD>(&flags),            		// LPDWORD lpFlags
					reinterpret_cast<sockaddr*>(&remoteIP), 	        // sockaddr* lpFrom
					reinterpret_cast<LPINT>(&remoteIPlen),         		// LPINT lpFromlen
					reinterpret_cast<LPWSAOVERLAPPED>(&overlapped), 	// LPWSAOVERLAPPED lpOverlapped
					callback                                       			// LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
				);
	if (result == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
        if (errorCode != WSA_IO_PENDING)
        {
            std::cerr << "Failed to start receive operation: " << errorCode << std::endl;
            // Handle receive error
            // ...
        }
	}

    //////////////////////////////idk if that's necessary here: buffer[result] = 0; //make the buffer zero terminated, marking end of string
    return result;
}


/// @todo error handling
void UDPSocket::Bind(sockaddr_in &localIP, unsigned short port) {
    /*
    sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = htonl(INADDR_ANY); //allows to bind to any available local network interface
    add.sin_port = htons(port); //htons converts port number from host byte order to network byte order

    int ret = bind(sock, reinterpret_cast<SOCKADDR*>(&add), sizeof(add)); //bind sock to add (cast to a sockaddr pointer) and port
    if (ret < 0) //== SOCKET_ERROR
        throw std::system_error(WSAGetLastError(), std::system_category(), "Bind failed");
    */

    int result = bind(sock, reinterpret_cast<sockaddr*>(&localIP), sizeof(localIP));

    if (result == SOCKET_ERROR) {
        int error = WSAGetLastError();
        // Handle error
    }
}


/// @todo error handling
void UDPSocket::Connect(sockaddr_in &remoteIP) {

    //the last four params are LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS
    //which can all be NULL if no specific caller data, callee data, and quality of service are required
    int result = WSAConnect(sock, reinterpret_cast<const sockaddr*>(&remoteIP), sizeof(remoteIP), NULL, NULL, NULL, NULL);
    if (result == SOCKET_ERROR) {
        int error = WSAGetLastError();
        // Handle error
    }
}
