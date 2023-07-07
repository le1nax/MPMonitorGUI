#include "include/SocketClient.h"
#include "include/Program.h"

#include <string.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <Windows.h>
#include <bitset>
#include <cstddef>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib") //link against the Winsock library



#define REMOTEIP "169.254.0.1"
#define REMOTEPORT 24105

#define MAX_BUFFER_SIZE 1380

using namespace std;

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

// Saves us from typing std::cout << etc. etc. etc.
using namespace std; 

// Main entry point into the server
int main()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
    {
		cout << "Can't start Winsock! " << wsOk;
		return -1;
	}

	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	// Create a server hint structure for the server
	sockaddr_in server;
	server.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	server.sin_family = AF_INET; // Address format is IPv4
	server.sin_port = htons(69696); // Convert from little to big endian

	// Try and bind the socket to the IP and port
	if (bind(in, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return -1;
	}

	////////////////////////////////////////////////////////////
	// MAIN LOOP SETUP AND ENTRY
	////////////////////////////////////////////////////////////

	sockaddr_in client; // Use to hold the client information (port / ip address)
	int clientLength = sizeof(client); // The size of the client information


/*
	// char buf[MAX_BUFFER_SIZE];

	// // Enter a loop
	// while (true)
	// {
	// 	ZeroMemory(&client, clientLength); // Clear the client structure
	// 	ZeroMemory(buf, MAX_BUFFER_SIZE); // Clear the receive buffer

	// 	// Wait for message
	// 	int bytesIn = recvfrom(in, buf, MAX_BUFFER_SIZE, 0, (sockaddr*)&client, &clientLength);
	// 	if (bytesIn == SOCKET_ERROR)
	// 	{
	// 		cout << "Error receiving from client " << WSAGetLastError() << endl;
	// 		continue;
	// 	}

	// 	// Display message and client info
	// 	char clientIp[256]; // Create enough space to convert the address byte array
	// 	ZeroMemory(clientIp, 256); // to string of characters

	// 	// Convert from byte array to chars
	// 	inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

	// 	// Display the message / who sent it
	// 	cout << "Message recv from " << clientIp << " : " << buf << endl;
	// }
*/


    // Buffer for receiving messages
    char buffer1[MAX_BUFFER_SIZE];
    int len = MAX_BUFFER_SIZE;

    LPWSABUF lpwsabufferServer = new WSABUF();
    lpwsabufferServer->len = sizeof(buffer1);

    DWORD bytesReceived1 = 0;
    DWORD flags = 0;


	while (true)
	{
		ZeroMemory(&client, clientLength); // Clear the client structure
		ZeroMemory(buffer1, MAX_BUFFER_SIZE); // Clear the receive buffer

    	lpwsabufferServer->buf = buffer1;


		// Receive from Server
		if (WSARecvFrom(in, lpwsabufferServer, 1, &bytesReceived1, &flags,
						reinterpret_cast<sockaddr*>(&client), reinterpret_cast<LPINT>(&clientLength), nullptr, nullptr) == SOCKET_ERROR)
		{
			std::cerr << "WSARecvFrom failed. Error code: " << WSAGetLastError() << std::endl;
			continue;
		}

		std::cout << "Received " << bytesReceived1 << " Bytes" << std::endl;


		// Process received message
		// std::string receivedMessage(lpwsabufferServer->buf, bytesReceived1); //////////// this line doesn't work

		// Display message and client info
		char clientIp[256]; // Create enough space to convert the address byte array
		ZeroMemory(clientIp, 256); // to string of characters

		// Convert from byte array to chars
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		// Display the message / who sent it
		cout << "Message WSArecv from " << clientIp << " : " << lpwsabufferServer->buf << endl;

	}



    ///////////////////////////////////////////////////////////////////
    //// CLEANUP
    ///////////////////////////////////////////////////////////////////

	// delete WSA Buffer Pointer
	delete lpwsabufferServer;

	// Close socket
	closesocket(in);

	// Shutdown winsock
	WSACleanup();
    return 0;
}


