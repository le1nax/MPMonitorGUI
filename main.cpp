#include "include/SocketClient.h"
#include "include/Program.h"

#include <string.h>
#include <thread>
#include <iostream>
#include <chrono>


#define REMOTEIP "169.254.0.1"
#define REMOTEPORT 24105

#define MAX_BUFFER_SIZE 1380

using namespace std;

/*
int main()
{
    try
   {
        unique_ptr<WSASession> Session = make_unique<WSASession>();

        const std::string s_remoteIP = REMOTEIP;
        const unsigned short remotePort = REMOTEPORT;
        unique_ptr<SocketClient> client = make_unique<SocketClient>(s_remoteIP, remotePort);

        //ppgPath = "TEST.docx";
        //ppgFilename = "TEST.docx";

        //var parser = new CommandLineParser(); /////////////////
        //parser.Parse(argv);

        client->establishLanConnection();
    }
    catch (std::exception &ex) //catch any occurring system errors
    {
        std::cout << ex.what();  //print error message
    }
    char c;
    std::cin >> c; // Wait for user input before exiting

   
    return 0;
}
*/


// Function to be executed in the receive thread
void Receive(SOCKET& serverSocket, SOCKET& clientSocket, LPWSABUF& lpwsabuffer, DWORD& bytesReceived1, DWORD& flags, sockaddr_in& serverAddress, int serverAddressSize) 
{

    std::cout << "Thread Receive: Starting work" << std::endl;

    std::chrono::milliseconds duration2(2000); // 5000 = 5 seconds
    std::this_thread::sleep_for(duration2);

    if (WSARecvFrom(clientSocket, lpwsabuffer, 1, &bytesReceived1, &flags,
                    reinterpret_cast<sockaddr*>(&serverAddress), reinterpret_cast<LPINT>(&serverAddressSize), nullptr, nullptr) == SOCKET_ERROR)
    {
        std::cerr << "WSARecvFrom failed. Error code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        delete lpwsabuffer;
        return;
    }

    std::cout << "Received " << bytesReceived1 << " Bytes" << std::endl;

    std::cout << "Thread Receive: Work completed" << std::endl;

}


// Function to be executed in the send thread
void Send(SOCKET& serverSocket, SOCKET& clientSocket, LPWSABUF& lpwsabuffer, DWORD& bytesSent, DWORD& flags, sockaddr_in& clientAddress, int clientAddressSize) 
{
    std::cout << "Thread Send: Starting work" << std::endl;

    int resultBytesSent = WSASendTo(serverSocket, lpwsabuffer, 1, &bytesSent, flags,
                reinterpret_cast<sockaddr*>(&clientAddress), clientAddressSize, nullptr, nullptr);
    if (resultBytesSent == SOCKET_ERROR)
    {
        std::cerr << "WSASendTo failed. Error code: " << WSAGetLastError() << std::endl;
        std::cerr << "# bytes sent in WSASendTo: " << resultBytesSent << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        delete lpwsabuffer;
        return;
    } else { std::cout << "Sent " << bytesSent << " Bytes" << std::endl; }

    //std::cout << "in the wsa fct: bytesSent = " << bytesSent << std::endl;
    //std::cout << "result of the wsa fct: resultBytesSent = " << bytesSent << std::endl;

    //std::cout << "Sent response to client" << std::endl;

    std::cout << "Thread Send: Work completed" << std::endl;
}




int main()
{
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return -1;
    }

    // Create client socket
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create client socket" << std::endl;
        WSACleanup();
        return -1;
    }

    // Create server socket
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create server socket" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Create client address structure
    sockaddr_in clientAddress{};
    int clientAddressSize = sizeof(clientAddress);

    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Bind to all available interfaces
    if (clientAddress.sin_addr.s_addr == INADDR_NONE)
    {
        std::cerr << "The target ip address entered must be a legal IPv4 address." << std::endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Create server address structure
    sockaddr_in serverAddress{};
    int serverAddressSize = sizeof(serverAddress);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY; // Bind to all available interfaces
    serverAddress.sin_port = htons(12345);

    if (serverAddress.sin_addr.s_addr == INADDR_NONE)
    {
        std::cerr << "The target ip address entered must be a legal IPv4 address." << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Resolve server hostname
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0)
    {
        std::cerr << "Invalid address" << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
    
    // Bind server socket to an IP address and port
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "bind of server socket and address failed. Error code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }





    std::cout << "Server is listening for incoming messages..." << std::endl;

    // Buffer for receiving messages
    char buffer1[MAX_BUFFER_SIZE];
    LPWSABUF lpwsabuffer = new WSABUF(); //std::unique_ptr<WSABUF> wsabuffer = std::make_unique<WSABUF>();
    lpwsabuffer->buf = buffer1;
    lpwsabuffer->len = sizeof(buffer1);
                                         //LPWSABUF lpwsabuffer = wsabuffer.get();


    DWORD bytesReceived1 = 0;
    DWORD flags = 0;

/*
    // Receive message from client
    // std::thread ReceiveThread([&]() 
    // {


    // // if (WSARecvFrom(serverSocket, lpwsabuffer, 1, &bytesReceived1, &flags,
    // //                 reinterpret_cast<sockaddr*>(&clientAddress), reinterpret_cast<LPINT>(&clientAddressSize), nullptr, nullptr) == SOCKET_ERROR)
    // // {
    // //     std::cerr << "WSARecvFrom failed. Error code: " << WSAGetLastError() << std::endl;
    // //     closesocket(serverSocket);
    // //     WSACleanup();
    // //     return -1;
    // // }

    // });
    // ReceiveThread.detach();
*/

    // Receive message from client
	std::thread ReceiveThread([&]() 
    {
        Receive(serverSocket, clientSocket, lpwsabuffer, bytesReceived1, flags, serverAddress, serverAddressSize);
    });

    // Prepare response
    char buffer2[MAX_BUFFER_SIZE];

    std::string response = "Hello from server!";
    strncpy_s(buffer2, response.c_str(), sizeof(buffer2)); //copy string into buffer

    lpwsabuffer->buf = buffer2;
    lpwsabuffer->len = sizeof(buffer2);

    DWORD bytesSent = 0;
    
/*
    // Send response back to the client
    // std::thread SendThread([&]() 
    // {

    //     // int resultBytesSent = WSASendTo(serverSocket, lpwsabuffer, 1, &bytesSent, flags,
    //     //             reinterpret_cast<sockaddr*>(&clientAddress), clientAddressSize, nullptr, nullptr);
    //     // if (resultBytesSent == SOCKET_ERROR)
    //     // {
    //     //     std::cerr << "WSASendTo failed. Error code: " << WSAGetLastError() << std::endl;
    //     //     std::cerr << "# bytes sent in WSASendTo: " << resultBytesSent << std::endl;
    //     //     closesocket(serverSocket);
    //     //     closesocket(clientSocket);
    //     //     WSACleanup();
    //     //     return -1;
    //     // }

    // });
    // SendThread.detach();
*/

    // Send response back to the client
	std::thread SendThread([&]() 
    {
        Send(serverSocket, clientSocket, lpwsabuffer, bytesSent, flags, clientAddress, clientAddressSize);
    });
    


    // Wait for a specified amount of time
    std::chrono::milliseconds duration2(10000); // 5000 = 5 seconds
    std::this_thread::sleep_for(duration2);

    // Wait for both threads to finish their work
    ReceiveThread.join();
    SendThread.join();


    // Process received message
    std::string receivedMessage(lpwsabuffer->buf, bytesReceived1);
    std::cout << "Received message from client: " << receivedMessage << std::endl;









/*

    // Receive response from server
    int serverAddressSize = sizeof(serverAddress);
    DWORD bytesReceived2 = 0;
    flags = 0;
    if (WSARecvFrom(clientSocket, lpwsabuffer, 1, &bytesReceived2, &flags,
                    reinterpret_cast<sockaddr*>(&serverAddress), reinterpret_cast<LPINT>(&serverAddressSize), nullptr, nullptr) == SOCKET_ERROR)
    {
        std::cerr << "WSARecvFrom failed. Error code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        delete lpwsabuffer;
        return -1;
    }

    // Send message to server
    char buffer3[MAX_BUFFER_SIZE];

    std::string message = "Hello from client!";
    strncpy_s(buffer3, message.c_str(), sizeof(buffer3));

    lpwsabuffer->buf = buffer3;
    lpwsabuffer->len = sizeof(buffer3);

    if (WSASendTo(clientSocket, lpwsabuffer, 1, &bytesSent, flags,
                  reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress), nullptr, nullptr) == SOCKET_ERROR)
    {
        std::cerr << "Send error" << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        delete lpwsabuffer;
        return -1;
    }

    std::cout << "Message sent to server" << std::endl;

    // Process received response
    std::string receivedResponse(lpwsabuffer->buf, bytesReceived2);
    std::cout << "Received response from server: " << receivedResponse << std::endl;

*/

    // Cleanup
    delete lpwsabuffer;
    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();
    std::cout << "Closed WSASession" << std::endl;

    return 0;
}

