#include <winsock2.h>
#include <string>
#pragma comment(lib, "Ws2_32.lib")



/*
* Definition of class methods for UDPClient. 
* Provides a simply UDP socket where a request can be issued to the server and a reply is received.
* For futher information about socket for Windows see Microsoft documentation about Winsock.
*/

class UDPClient{
public:
	UDPClient(std::string ipAddress, int port);
	std::string getIPAdress();
	int getPort();
	int recvResult;
	void UDPRequest(char* reqMessage);
	void UDPReceive(char* recvBuff);
	
protected:
private:
	SOCKET createSocket();
	void connectSocket();
	std::string ipAddress;
	int port;
	SOCKET udpSocket;

};