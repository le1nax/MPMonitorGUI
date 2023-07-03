//#include "../include/SocketMonitor.h"
//#include <iostream>

//

//just saving this bc of the SOCKET_ERROR and closesocket(socket) stuff for error handling

/*
void SocketMonitor::startServer(int port) {

    //bind
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(socketDescriptor, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind UDP socket." << std::endl;
        closesocket(socketDescriptor);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    //data exchange
    char buffer[1024];
    sockaddr_in clientAddress{};
    int clientAddressSize = sizeof(clientAddress);
    int receivedBytes;

    while (true) {
        receivedBytes = recvfrom(socketDescriptor, buffer, sizeof(buffer), 0,
            reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);

        if (receivedBytes == SOCKET_ERROR) {
            std::cerr << "Failed to receive data." << std::endl;
            closesocket(socketDescriptor);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        std::cout << "Received data from " << inet_ntoa(clientAddress.sin_addr) << ": " << buffer << std::endl;
    }
}
*/