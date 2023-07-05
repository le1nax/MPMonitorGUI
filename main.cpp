#include "include/SocketClient.h"
#include "include/Program.h"


#define REMOTEIP "169.254.0.1"
#define REMOTEPORT 24105

using namespace std;

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