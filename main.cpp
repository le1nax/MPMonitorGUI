
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
        // setup remote server address structure
        sockaddr_in remoteIP;
        memset((void *)&remoteIP, '\0', sizeof(struct sockaddr_in));
        
        const std::string s_remoteIP = REMOTEIP;
            //const unsigned short remotePort = REMOTEPORT;
        remoteIP.sin_family = AF_INET; 
            //remoteIP.sin_port = htons(remotePort); 
        inet_pton(AF_INET, s_remoteIP.c_str(), &(remoteIP.sin_addr));

        unique_ptr<SocketClient> client = make_unique<SocketClient>();

        client->Connect(remoteIP);
        // program.m_client.Connect(reinterpret_cast<const sockaddr*>(&remoteIP)); ///////////////////
        // program.m_client.Connect((sockaddr)(&remoteIP))

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