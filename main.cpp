//#include "include/ControllerBox.h"
//#include "include/MPMonitor.h"
//#include "include/MPUDPMonitor.h"
//#include "include/UDPClient.h"
#include "include/SocketClient.h"
//#include "include/Program.h"
#include <iostream>


#define REMOTEIP ""
    //#define REMOTEPORT 0


int main(int argc, char *argv[])
{
    try
    {
        // setup remote server address structure
        sockaddr_in remoteIP;
        const std::string s_remoteIP = REMOTEIP;
            //const unsigned short remotePort = REMOTEPORT;
        remoteIP.sin_family = AF_INET; 
            //remoteIP.sin_port = htons(remotePort); 
        inet_pton(AF_INET, s_remoteIP.c_str(), &(remoteIP.sin_addr));

        memset((void *)&remoteIP, '\0', sizeof(struct sockaddr_in));

        WSASession Session;
        SocketClient client;
        Program program(client);

        //program.m_client.Connect(&remoteIP);
        //program.m_client.Connect(reinterpret_cast<const sockaddr*>(&remoteIP)); ///////////////////
        //program.m_client.Connect((sockaddr)(&remoteIP));


        std::cout << "Reading from Philips Monitor..." << std::endl;

        std::string ppgPath;
        std::string ppgFilename;

        ppgPath = argv[1];
        ppgFilename = argv[2];

        //ppgPath = "TEST.docx";
        //ppgFilename = "TEST.docx";

        //var parser = new CommandLineParser(); /////////////////
        //parser.Parse(argv);

        //program.establishLanConnection();


    }
    catch (std::exception &ex) //catch any occurring system errors
    {
        std::cout << ex.what();  //print error message
    }
    char c;
    std::cin >> c; // Wait for user input before exiting


    return 0;
}