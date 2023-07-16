#include "include/SocketClient.h"
#include "include/Program.h"

#include <string.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <stdlib.h> 


#define REMOTEIP "169.254.0.1"
#define REMOTEPORT 24105

#define MAX_BUFFER_SIZE 2001

using namespace std;

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

// Saves us from typing std::cout << etc. etc. etc.
using namespace std; 

// Main entry point into the server
int main()
{
	////////////////////////////////////////////////////////////
	//// INITIALISATION
	////////////////////////////////////////////////////////////

	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
    {
		cout << "Can't start Winsock! " << wsOk;
		return -1;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

	// Create a server hint structure for the server
	sockaddr_in server;
	server.sin_addr.S_un.S_addr = ADDR_ANY; // Use any IP address available on the machine
	server.sin_family = AF_INET; // Address format is IPv4
	server.sin_port = htons(69696); // Convert from little to big endian

	// Try and bind the socket to the IP and port
	if (bind(serverSocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return -1;
	}

	// Create client address structure
    sockaddr_in clientAddress{};
    int clientAddressSize = sizeof(clientAddress);

    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddress.sin_port = htons(69696);

    if (clientAddress.sin_addr.s_addr == INADDR_NONE)
    {
        std::cerr << "The target ip address entered must be a legal IPv4 address." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Resolve client hostname
    if (inet_pton(AF_INET, "127.0.0.1", &(clientAddress.sin_addr)) <= 0)
    {
        std::cerr << "Invalid address" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

	bool sent = FALSE;
	bool recv = FALSE;

	// Enter a loop
	while (69 < 6969)
	{
		Sleep(2001);	


		////////////////////////////////////////////////////////////
		//// SERVER RECEIVES via WSARecvFrom
		////////////////////////////////////////////////////////////

		sockaddr_in clientRecv; // Use to hold the client information (port / ip address)
		int clientRecvLength = sizeof(clientRecv); // The size of the client information

		char buffer1[MAX_BUFFER_SIZE];
		LPWSABUF lpwsabufferServer = new WSABUF();		
		lpwsabufferServer->len = sizeof(buffer1);

		DWORD bytesReceived1 = 0;
		DWORD flags2 = 0;

		if (!recv)
		{
			ZeroMemory(&clientRecv, clientRecvLength); // Clear the client structure
			ZeroMemory(buffer1, MAX_BUFFER_SIZE); // Clear the receive buffer
			lpwsabufferServer->buf = buffer1;

			std::cout << "server waiting to receive" << std::endl;
						// Receive from Client
			if (WSARecvFrom(serverSocket, lpwsabufferServer, 1, &bytesReceived1, &flags2,
							reinterpret_cast<sockaddr*>(&clientRecv), reinterpret_cast<LPINT>(&clientRecvLength), nullptr, nullptr) == SOCKET_ERROR)
			{
				std::cerr << "WSARecvFrom failed. Error code: " << WSAGetLastError() << std::endl;
				closesocket(serverSocket);
				WSACleanup();
				delete lpwsabufferServer;
				continue;
			}

			// Display message and client info
			char clientIp[256]; // Create enough space to convert the address byte array
			ZeroMemory(clientIp, 256); // to string of characters

			// Convert from byte array to chars
			inet_ntop(AF_INET, &clientRecv.sin_addr, clientIp, 256);

			// Display the message / who sent it
			cout << "Message recv from " << clientIp << " : " << lpwsabufferServer->buf << endl;
		
			recv = TRUE;
			delete lpwsabufferServer;
		}

		////////////////////////////////////////////////////////////
		//// SERVER SENDS
		////////////////////////////////////////////////////////////
		std::cin.get();

		if (!sent) {
			string s("Server sendet was"); //string to send
			cout << "Server sendet was" << endl;

			char buffer[MAX_BUFFER_SIZE];
			std::strcpy(buffer, s.c_str());
			
			LPWSABUF lpwsabuf = new WSABUF();
			lpwsabuf->buf = buffer;
			lpwsabuf->len = sizeof(buffer);

			DWORD numBytesSent = 0;
			DWORD flags = 0;

			int wsaSendOK = WSASendTo(serverSocket, lpwsabuf, 1, reinterpret_cast<LPDWORD>(&numBytesSent), flags,
						reinterpret_cast<sockaddr*>(&clientRecv), sizeof(clientRecv), nullptr, nullptr);
			if (wsaSendOK == SOCKET_ERROR)
			{
				cout << "WSASendTo didn't work! " << WSAGetLastError() << endl;
			}

			sent = TRUE;
			delete lpwsabuf;
		}

	}
	
	// Close socket
	closesocket(serverSocket);

	// Shutdown winsock
	WSACleanup();
    return 0;
}


