#include "include/SocketClient.h"
#include "include/Program.h"


#define REMOTEIP "169.254.0.1"
#define REMOTEPORT 24105

using namespace std;

#define MAX_BUFFER_SIZE 1024

int main()
{
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return -1;
    }

    // Create server socket
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return -1;
    }

    // Bind server socket to an IP address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
    serverAddress.sin_port = htons(12345);     // Port number
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server is listening for incoming messages..." << std::endl;

    // Client address structure
    sockaddr_in clientAddress{};
    int clientAddressSize = sizeof(clientAddress);

    // Buffer for receiving messages
    char buffer[MAX_BUFFER_SIZE];

    // Receive message from client
    DWORD bytesReceived = 0;
    DWORD flags = 0;
    if (WSARecvFrom(serverSocket, reinterpret_cast<LPWSABUF>(&buffer), 1, &bytesReceived, &flags,
                    reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize, nullptr, nullptr) == SOCKET_ERROR)
    {
        std::cerr << "Receive error" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Process received message
    std::string receivedMessage(buffer, bytesReceived);
    std::cout << "Received message from client: " << receivedMessage << std::endl;

    // Prepare response
    std::string response = "Hello from server!";
    strncpy_s(buffer, response.c_str(), sizeof(buffer));

    // Send response back to the client
    DWORD bytesSent = 0;
    if (WSASendTo(serverSocket, reinterpret_cast<LPWSABUF>(&buffer), 1, &bytesSent, flags,
                  reinterpret_cast<sockaddr*>(&clientAddress), clientAddressSize, nullptr, nullptr) == SOCKET_ERROR)
    {
        std::cerr << "Send error" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Sent response to client" << std::endl;

    // Create client socket
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Server address structure
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Port number

    // Resolve server hostname
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr)) <= 0)
    {
        std::cerr << "Invalid address" << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Send message to server
    std::string message = "Hello from client!";
    strncpy_s(buffer, message.c_str(), sizeof(buffer));

    // Send message to server
    if (WSASendTo(clientSocket, reinterpret_cast<LPWSABUF>(&buffer), 1, &bytesSent, flags,
                  reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr), nullptr, nullptr) == SOCKET_ERROR)
    {
        std::cerr << "Send error" << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Message sent to server" << std::endl;

    // Receive response from server
    int serverAddressSize = sizeof(serverAddr);
    DWORD bytesReceived = 0;
    flags = 0;
    if (WSARecvFrom(clientSocket, reinterpret_cast<LPWSABUF>(&buffer), 1, &bytesReceived, &flags,
                    reinterpret_cast<sockaddr*>(&serverAddr), &serverAddressSize, nullptr, nullptr) == SOCKET_ERROR)
    {
        std::cerr << "Receive error" << std::endl;
        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Process received response
    std::string receivedResponse(buffer, bytesReceived);
    std::cout << "Received response from server: " << receivedResponse << std::endl;

    // Cleanup
    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}