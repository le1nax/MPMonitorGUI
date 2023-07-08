#include "include/SocketClient.h"
#include "include/Program.h"

#include <string.h>
#include <thread>
#include <iostream>
#include <chrono>

#pragma comment (lib, "ws2_32.lib")


#define REMOTEIP "169.254.0.1"
#define REMOTEPORT 24105

#define LOCALIP "127.0.0.1"
#define LOCALPORT 69696

#define MAX_BUFFER_SIZE 1380

using namespace std;

int main()
{
    try
   {
        unique_ptr<WSASession> Session = make_unique<WSASession>();

        const std::string s_remoteIP = REMOTEIP;
        const unsigned short remotePort = REMOTEPORT;
        unique_ptr<SocketClient> client = make_unique<SocketClient>(s_remoteIP, remotePort);

        client->establishLanConnection();
    }
    catch (std::exception &ex) //catch any occurring system errors
    {
        std::cout << ex.what();  //print error message
    }

    std::cin.get();

    return 0;
}

