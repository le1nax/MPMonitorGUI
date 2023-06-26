#include "../include/SocketClient.h"
#include <iostream>
#include <thread>

using namespace std;

SocketClient::SocketClient() : UDPSocket()
{
	///////////////////////////get sockaddr_in m_remoteIP from std::string m_remoteIPtarget

	memset(buffer, 0, sizeof(buffer)); //clear the buffer

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

void SocketClient::BeginReceive() {

	UdpState temp_client_state;
	temp_client_state.state_ip = m_sa_remoteIPtarget;
	temp_client_state.state_client = this;

	sockaddr_in temp_sa = RecvFrom(m_sa_remoteIPtarget, buffer);
	


	//....
}


void SocketClient::ReceiveCallback(std::uint32_t errorCode, std::uint32_t bytesReceived, LPWSAOVERLAPPED overlapped, std::uint32_t flags)
{
	/* ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (errorCode == 0)
    {
        // Data received successfully, process it
        if (bytesReceived > 0)
        {
            // Retrieve the buffer from the overlapped structure
            char* buffer = reinterpret_cast<char*>(overlapped->Pointer);

			// Data received successfully, process it
			if (bytesReceived > 0)
			{
				// Convert buffer to std::vector<byte> if needed
				std::vector<byte> data(buffer, buffer + bytesReceived);

				// Write data to file
				ByteArrayToFile(path, data.data(), bytesReceived);

				// Process data
				ReadData(data);
			}
        }
    }
    else
    {
        std::cout << "Receive error: " << errorCode << std::endl;
		// Handle receive error
		// ...
    }*/

    // Continue receiving data
    //BeginReceive();
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

	//	try //corresponds to line 509 in Program.cs
	//	{
	
		//_MPudpclient.Connect(_MPudpclient.m_remoteIPtarget);
		//new udpstate aka client und ip mit client=_mpudpclient und remoteIP=_MPudpclient.m_remoteIPtarget
				//_MPudpclient.Connect(_MPudpclient.m_remoteIPtarget);

            //Send MDSCreateEventResult message
                    //_MPudpclient.ProcessPacket(readmdsconnectbuffer);

                    //Send PollDataRequest message
                    //_MPudpclient.SendPollDataRequest();

                    //Send Extended PollData Requests cycled every second
                   // Task.Run(() => _MPudpclient.SendCycledExtendedPollDataRequest(nInterval));
                    //Console.WriteLine("POLL data REQUESTED");

                    //WaitForSeconds(1);

		//create string path to "_Philips_MPrawoutput.txt" in the current directory
				//string path = Path.Combine(Directory.GetCurrentDirectory(), Globals.pathFile+ "_Philips_MPrawoutput.txt"); //IB


		//Receive AssociationResult message (aka SOC buffer) and MDSCreateEventReport message
			//readassocbuffer = RecvFrom();
				// _MPudpclient.ByteArrayToFile(path, readassocbuffer, readassocbuffer.GetLength(0));


		//Receive ARRAY from buffer
			//readmdsconnectbuffer = _MPudpclient.Receive(ref _MPudpclient.m_remoteIPtarget);
				// _MPudpclient.ByteArrayToFile(path, readmdsconnectbuffer, readmdsconnectbuffer.GetLength(0));


		//Send MDSCreateEventResult message
				//_MPudpclient.ProcessPacket(readmdsconnectbuffer);


		//Send Extended PollData Requests cycled every second
				//Task.Run(() => _MPudpclient.SendCycledExtendedPollDataRequest(nInterval));


		/*if (nWaveformSet != 0)
		{
			_MPudpclient.GetRTSAPriorityListRequest();
			if (nWaveformSet != 11) 

						//what is nwaveformset? why 11? possible reasons with 11 in guide:

						//header information p.30: The protocol_id field contains ID and version information.
						//It can be used to define different service access points. Data Export uses the ID 0x11.

						//The StringFormat describes how strings are encoded. The IntelliVue monitor uses unicode encoding.
						//typedef u_16 StringFormat; #define STRFMT_UNICODE_NT 11

						//Num 11 Placeholder for Vuelink Flex Text
						//Label: NLS_VUELINK_FLX1_NPS_TEXT_NUM11 

						//alert code NOM_EVT_EXT_DEV_AL_CODE_11

						//AbortSessionData
						//0x11 0x01 0x03


			{
				_MPudpclient.SetRTSAPriorityList(nWaveformSet); //wave object priority list specifies processing order of wave objects/data
			}

			Task.Run(() => _MPudpclient.SendCycledExtendedPollWaveDataRequest(nInterval)); //method executed asynchronously in the background thread pool thread

						//The (cycled, repetitive) request aims to retrieve waveform data (a sequence of values that represent a waveform or signal) and may encompass 
						//additional info or extended data associated with the waveforms (metadata, timestamps, measurement parameters, etc)
						
						//By using Task.Run(), the given code snippet creates a new task that runs the lambda expression in a separate background thread. 
						//It enables the code to run asynchronously, allowing other tasks or operations to continue without waiting for the completion of the specified method.

						//(parameters) => {expression}
						//Lambda expressions are often used in situations where a short, inline function is required. 
						//Instead of declaring a separate method or delegate, a lambda expression allows you to define the functionality right at the point where it is needed.

						//in c++: 	std::thread([&]() { _MPudpclient.SendCycledExtendedPollWaveDataRequest(nInterval); }).detach();
		}*/


////////////////////////////////////////////////  :))  //////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//Recheck MDS Attributes	//Task.Run(() => _MPudpclient.RecheckMDSAttributes(nInterval));				
		std::thread recheckMDSAttributesThread([&]() {	
			//RecheckMDSAttributes(nInterval);
			RecheckMDSAttributes();

		});
		recheckMDSAttributesThread.detach(); //allows the thread to continue executing independently, without requiring synchronization or joining with the parent thread

				//lambda functions in C++:	[capture-list](parameters) -> return-type { function-body }
				//capture-list being the variables from the enclosing scope that you want to have accessible within the lambda function, can be captured by value, reference, or a mix of both

				//possibly [&] instead of [this] --> & indicates that you want to capture all variables from the enclosing scope by reference, 
				//this for capturing all the class members by ref so you can work w the class members within the lambda body as if you were inside a member function of the class


		//Keep Connection Alive		//Task.Run(() => _MPudpclient.KeepConnectionAlive(nInterval));
		std::thread keepConnectionAliveThread([&]() {
			//KeepConnectionAlive(nInterval);
			KeepConnectionAlive();
		});
		keepConnectionAliveThread.detach();


		//Receive PollDataResponse message / Receive poll data		//_MPudpclient.BeginReceive(new AsyncCallback(ReceiveCallback), state);
		std::thread receiveThread([&]() {
    		//BeginReceive(ReceiveCallback, state); //initiates the asynchronous receiving of data, ReceiveCallback() as the callback fct to be executed whenever data is received
			BeginReceive();

		});
		receiveThread.detach();

				//It's important to note that the ReceiveCallback function should be implemented in a way that it can handle multiple invocations and 
				//process each packet of data independently. The function should not rely on any assumptions about the order or timing of packet arrivals, 
				//as they can occur in an unpredictable manner due to the asynchronous nature of the UDP communication.


////////////////////////////////////////////////  :))  //////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 	} catch (Exception ex) {
	//			Console.WriteLine("Error opening/writing to UDP port :: " + ex.Message, "Error!");
	//          Console.Beep();
	//	} 
	
	// ... //






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

