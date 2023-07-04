#include "../include/Socket.h"
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
    sock = INVALID_SOCKET;
    if (sock == WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_OVERLAPPED)== INVALID_SOCKET)
    {
        throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket");
    }
   // Fehler

/*
   I/O
*/
//     //AF_INET = IPv4; SOCK_DGRAM = Byte stream for UDP; IPPROTO_UDP = UDP Protocol
//     sock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//     if (sock == INVALID_SOCKET)
//         throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket");
//         //std::cerr << "Failed to create socket. Error: " << WSAGetLastError() << std::endl;
}


UDPSocket::~UDPSocket() {
    closesocket(sock);
}


long unsigned int UDPSocket::SendTo(sockaddr_in& remoteIP, const char* buffer, long unsigned int flags) {
    //send data provided in buffer to receiver address

    long unsigned int numBytesSent = 0; //will in the end contain the number of bytes sent and be returned

    WSAOVERLAPPED overlapped;
    overlapped.hEvent = WSACreateEvent();
    if (overlapped.hEvent == WSA_INVALID_EVENT) 
    {
        std::cerr << "WSACreateEvent in SendTo() failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }

    int result = WSASendTo(
					sock,                                       		// SOCKET s
					reinterpret_cast<LPWSABUF>(&buffer),          		// LPWSABUF lpBuffers
					1,                                            		// DWORD dwBufferCount
					reinterpret_cast<LPDWORD>(&numBytesSent),    		// LPDWORD lpNumberOfBytesSent
					reinterpret_cast<DWORD>(flags),            		    // DWORD dwFlags
					reinterpret_cast<const sockaddr*>(&remoteIP),       // const sockaddr* lpTo
                    sizeof(remoteIP),         		                    // int iTolen
					reinterpret_cast<LPWSAOVERLAPPED>(&overlapped), 	// LPWSAOVERLAPPED lpOverlapped
					NULL                                       			// LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
				);

	if (result == SOCKET_ERROR)
    {
		int errorCode = WSAGetLastError();
        if (errorCode != WSA_IO_PENDING)
        {
            std::cerr << "Failed to start send operation: " << errorCode << std::endl;
            WSACloseEvent(overlapped.hEvent);
            closesocket(sock);
            WSACleanup();
            return 0;
        }
	}

    DWORD temp = WaitForSingleObject(overlapped.hEvent, INFINITE); 
    if (temp == WAIT_FAILED)
    {
        std::cerr << "WaitForSingleObject in SendTo() failed with error: " << WSAGetLastError() << std::endl;
        WSACloseEvent(overlapped.hEvent);
        closesocket(sock);
        return 0;
    }

    /*DWORD temp = WSAWaitForMultipleEvents(1, &overlapped.hEvent, TRUE, INFINITE, TRUE);
    if (temp == WSA_WAIT_FAILED) 
    {
        std::cerr << "WSACreateEvent in SendTo() failed with error: " << WSAGetLastError() << std::endl;
        return 0;
    }*/

    temp = WSAGetOverlappedResult(sock, &overlapped, reinterpret_cast<LPDWORD>(&numBytesSent), FALSE, reinterpret_cast<LPDWORD>(&flags));
    if (temp == FALSE) 
    {
        std::cerr << "WSAGetOverlappedResult in SendTo() failed with error: " << WSAGetLastError() << std::endl;
        WSACloseEvent(overlapped.hEvent);
        closesocket(sock);
        return 0;
    }
    else
    {
        std::cout << "Number of sent bytes =  " << numBytesSent << std::endl;
    }

    return numBytesSent;
}


int UDPSocket::RecvFrom(sockaddr_in remoteIP, char* buffer, long unsigned int &numBytesReceived, LPWSAOVERLAPPED overlapped, long unsigned int flags, LPWSAOVERLAPPED_COMPLETION_ROUTINE callback ) {
//receive data into buffer from remote sender address

    int remoteIPlen = sizeof(remoteIP);

    int result = WSARecvFrom(
					sock,                                       		// SOCKET s
					reinterpret_cast<LPWSABUF>(&buffer),          		// LPWSABUF lpBuffers
					1,                                            		// DWORD dwBufferCount
					reinterpret_cast<LPDWORD>(&numBytesReceived),       // LPDWORD lpNumberOfBytesRecvd
					reinterpret_cast<LPDWORD>(flags),            		// LPDWORD lpFlags
					reinterpret_cast<sockaddr*>(&remoteIP), 	        // sockaddr* lpFrom
					reinterpret_cast<LPINT>(&remoteIPlen),         		// LPINT lpFromlen
					reinterpret_cast<LPWSAOVERLAPPED>(&overlapped), 	// LPWSAOVERLAPPED lpOverlapped
					callback                                            // LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
				);

	if (result == SOCKET_ERROR) {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            std::cerr << "Failed to initiate receive. Error: " << WSAGetLastError() << std::endl;
            WSACloseEvent(overlapped->hEvent);
			closesocket(sock);
            return SOCKET_ERROR;
        }
	}

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
