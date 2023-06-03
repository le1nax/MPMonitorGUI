#include "UDPClient.h"

/*
* Declaration of class methods for UDPClient. 
* Provides a simply UDP socket where a request can be issued to the server and a reply received.
* For futher information about socket for Windows see Microsoft documentation about Winsock.
*/



/*
* Constructor method for UDPClient
* Assigns IP address and port, creates a Winsock (IPv4/UDP) and connects to Socket
*/
UDPClient::UDPClient(std::string ipAddress, int port){
	
	
	this->ipAddress = ipAddress;
	this->port = port;
	this->udpSocket = createSocket();
	if(this->udpSocket == NULL){
		return;
	}
	connectSocket();
	
}


/*
* Method for creating a Winsock; returns Winsock (client socket)
*/
SOCKET UDPClient::createSocket(){
	
	// //Create a Socket if it is windows operating system (what do we need to do for UNIX?)
	// WSAData wsaData = {0};
	// WORD ver = MAKEWORD(2,2); // Request for Winsock version 2.2 on the system
	
	
	// int wsaResult = WSAStartup(ver, &data);
	
	// if(wsResult != 0){
	
	// 	std::cerr << "Winsock couldn't be started, Error #" << wsResult << std::endl;
	// 	return;
	
	// }
	
	// SOCKET clientSocket = socket(AF_INET, SOCK_DGRM, IPPROTO_UDP);
	// if(clientSocket == INVALID_SOCKET){
	
	// 	std:cerr << "Can't create socket, Error #" << WSAGetLastError() << std::endl;
	// 	WSACleanup();
	// 	return;
		
	// }
	
	// return clientSocket;
	
}


/*
* Method for connecting to Server
* connect to server and port defined previously
*/
void UDPClient::connectSocket(){

	// // define which server and which port to connect to 
	// sockaddr_in hint;
	// hint.sin_family = AF_INET;
	// hint.sin_port = htons(this->port);
	// inet_pton(AF_INET, this->ipAddress.c_str(), &hint.sin_addr);
	
	// // Connect to Monitor
	// int connResult = connect(this->udpSocket, (sockaddr*) &hint, sizeof(hint));
	// if(connResult == SOCKET_ERROR){
	
	// 	std::cerr << "Can't connect to server, Error #" << WSAGetLastError() << std::endl;
	// 	closesocket(this->udpSocket);
	// 	WSACleanup();
	// 	return;
	// }

}

/*
* Method for sending query to Server
* [in] char* reqMessage
*/
void UDPClient::UDPRequest(char* reqMessage, char* recvBuff){
	
	
	// // Send request (reqMessage) to server 
	// int sendResult = sent(this->udpSocket, reqMessage, sizeof(reqMessage), 0);
	// if(sendResult == SOCKET_ERROR){
		
	// 	std::cerr << "Send failed: Error #" << WSAGetLastError() << std::endl;
	// 	closesocket(this->udpSocket)
	// 	WSACleanup();
	// 	return;
		
	// }
	
}



/*
* Method for receiving reply of server
* [out] char* recBuff
*/
void UDPClient::UDPReceive(char* recvBuff){

	
	// // Receive reply of server (recvBuff)
	// this->recvResult = recv(this->udpSocket, recvBuff, sizeof(recvBuff), 0);
	// if (this->recvResult > 0){
	// 	continue;
	// }		
	// else if(this->recvResult == 0){
	// 	std::cerr << "Connection closed" << std::endl;
	// }
	// else{
	// 	std::cerr << "receive failed with error" << WSAGetLastError() << std::endl;
	// }
	
}



/*
* Method for reading the IP address of UDP socket
*/
std::string UDPClient::getIPAddress(){

	return this->ipAddress;

}

/*
* Method for reading the port of UDP socket
*/
int UDPClient::getPort(){
	
	return this->port;
}



