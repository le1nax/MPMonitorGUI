#include "../include/SocketClient.h"
#include <iostream>

using namespace std;

SocketClient::SocketClient() : UDPSocket()
{

}

void SocketClient::sendBytes(vector<std::byte> bytes)
{
     ///@todo prüfen ob das funktioniert
    size_t len = bytes.size();
    char* charBytes = reinterpret_cast<char*>(bytes.data()); 
    SendTo(m_remoteIPtarget, m_port, charBytes, len);
}

void SocketClient::SendWaveAssociationRequest()
{
        
}

void SocketClient::ProcessPacket(std::vector<std::byte>)
{

}

void SocketClient::SendCycledExtendedPollDataRequest()
{

}

void SocketClient::SendCycledExtendedPollWaveDataRequest()
{

}

void SocketClient::establishLanConnection() 
{
    //  try
    //     {
            SendWaveAssociationRequest();
            //Receive AssociationResult message and MDSCreateEventReport message
            //readassocbuffer = RecvFrom();
                    // _MPudpclient.ByteArrayToFile(path, readassocbuffer, readassocbuffer.GetLength(0));
                    //Console.WriteLine("Receiving SOC buffer");

            //readmdsconnectbuffer = _MPudpclient.Receive(ref _MPudpclient.m_remoteIPtarget);
                    // _MPudpclient.ByteArrayToFile(path, readmdsconnectbuffer, readmdsconnectbuffer.GetLength(0));
                    // //Console.WriteLine("Receiving ARRAY from buffer");

            //Send MDSCreateEventResult message
                    //_MPudpclient.ProcessPacket(readmdsconnectbuffer);

                    //Send PollDataRequest message
                    //_MPudpclient.SendPollDataRequest();

                    //Send Extended PollData Requests cycled every second
                   // Task.Run(() => _MPudpclient.SendCycledExtendedPollDataRequest(nInterval));
                    //Console.WriteLine("POLL data REQUESTED");

                    //WaitForSeconds(1);

                   

//     try
//     {
//         WSASession Session; //initialize the windows sockets api session
//         UDPSocket Socket; //create a udp socket
        
//         std::string data = "hello world"; //define an example string to send
//         char buffer[100]; //create a buffer to hold received data

//         Socket.SendTo("127.0.0.1", 100, data.c_str(), data.size()); // Send data to IP address and port 100
//         Socket.RecvFrom(buffer, 100); // Receive data of length 100 into the buffer
//         std::cout << buffer; // Output the received data
//     }
//     catch (std::exception &ex) //catch any occurring system errors
//     {
//         std::cout << ex.what();  //print error message
//     }
//     char c;
//     std::cin >> c; // Wait for user input before exiting
//}
}

