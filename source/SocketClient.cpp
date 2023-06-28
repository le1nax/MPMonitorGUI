#include "../include/SocketClient.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sstream>
#include <vector>

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
    //SendTo(m_remoteIPtarget, m_port, charBytes, len);///////////////////////////////////////////////////////////// use of old SendTo function, not compatible anymore
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

void SocketClient::BeginReceive(int flags = 0) {

	UdpState temp_client_state;
	temp_client_state.state_ip = m_sa_remoteIPtarget;
	temp_client_state.state_client = this;	

	WSAOVERLAPPED overlapped{};	
	memset(&overlapped, 0, sizeof(WSAOVERLAPPED));
	overlapped.hEvent = reinterpret_cast<HANDLE>(&temp_client_state); //= reinterpret_cast<HANDLE>(new UdpState{ sock, m_sa_remoteIPtarget });


	int receiveResult = RecvFrom(temp_client_state.state_ip, temp_client_state.state_client.buffer, &overlapped, flags, ReceiveCallback);
	if (receiveResult == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			std::cerr << "Failed to initiate receive. Error: " << WSAGetLastError() << std::endl;
			delete reinterpret_cast<UdpState*>(overlapped.hEvent);
			closesocket(sock);
			return;
		}
	}

	// Wait for the receive operation to complete
    if (WSAGetOverlappedResult(
            sock,                                               // SOCKET s
            reinterpret_cast<LPWSAOVERLAPPED>(&overlapped),     // LPWSAOVERLAPPED lpOverlapped
            reinterpret_cast<LPDWORD>(&numBytesReceived),    	// LPDWORD lpcbTransfer
            TRUE,                                               // BOOL fWait -- whether the function should wait until the overlapped operation is completed (true = wait, false = retrive results immediately)
            reinterpret_cast<LPDWORD>(&flags)))                 // LPDWORD lpdwFlags
    {
        // Data received successfully, process it
        if (numBytesReceived > 0)
        {
			// Receive operation completed successfully
			ReceiveCallback(0, numBytesReceived, &overlapped); //go and process data
        }
    }
    else
    {
        // Receive operation failed
        std::uint32_t errorCode = WSAGetLastError();
        if (errorCode != WSA_IO_PENDING) //WSA_IO_PENDING = overlapped operation is still in progress
        {
            // Handle error
            std::cout << "Receive error: " << errorCode << std::endl;
        }
    }

	// Clean up resources
	delete reinterpret_cast<UdpState*>(overlapped.hEvent);
}


/// @todo error handling
/// @todo add path_to_file
/// @todo version A (as is now, with string) or B (commented out, with bytes)?
void CALLBACK SocketClient::ReceiveCallback(DWORD errorCode, DWORD numBytesReceived /*cbTransferred*/, LPWSAOVERLAPPED overlapped, DWORD flags = 0)
{
    if (errorCode == 0) 
    {
		UdpState udpState = *static_cast<UdpState*>(overlapped->hEvent);
				//or pointer? //UdpState* udpState = reinterpret_cast<UdpState*>(overlapped->hEvent);

		//read the number of bytes received from the buffer into receivedData
        std::string receivedData(udpState.state_client.buffer, numBytesReceived); 

		// Convert buffer to std::vector<byte> if needed
	// B: std::vector<std::byte> data_bytes(udpState.state_client.buffer, udpState.state_client.buffer + numBytesReceived);

	//im cs code: string path = Path.Combine(Directory.GetCurrentDirectory(),Globals.pathFile+"_Philips_MPrawoutput.txt");
		std::string path_to_file = ""; //////////////////////////////////////////////////

		// Write data to file
		bool writing_to_file_successful = ByteArrayToFile(path_to_file, receivedData);
	// B: bool writing_to_file_successful2 = ByteArrayToFile(path_to_file, data_bytes, numBytesReceived);

    }
    else
    {
        std::cerr << "ReceiveCallback error: " << errorCode << std::endl;
        // Handle the error
        // ... //
    }
}


bool SocketClient::ByteArrayToFile(const std::string& path_to_file, const std::string& bytes_string)
{
    try
    {
        // Open file for writing
        std::ofstream outFile(path_to_file, std::ios::app);
        if (outFile.is_open())
        {
            // Write data to file
            outFile << bytes_string << std::endl;

            // Close file stream
            outFile.close();

            return true;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in process: " << e.what() << std::endl;
    }

    // Error occurred, return false
    return false;
}


bool SocketClient::ByteArrayToFile(const std::string& path_to_file, const std::vector<std::byte>& data_bytes, uint32_t numBytesReceived)
{
    try
    {
        // Open file for writing
        std::ofstream outFile(path_to_file, std::ios::app);
        if (outFile.is_open())
        {
            // Convert byte array to hex string representation
            std::stringstream data_to_string;
            for (int i = 0; i < numBytesReceived; i++)
            {
                data_to_string << std::hex << static_cast<int>(data_bytes[i]);
                if (i != numBytesReceived - 1) data_to_string << "-";
            }
            std::string datastr = data_to_string.str();

            // Write data to file
            outFile << datastr << std::endl;

            // Close file stream
            outFile.close();

            return true;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in process: " << e.what() << std::endl;
    }

    // Error occurred, return false
    return false;
}


void SocketClient::RecheckMDSAttributes(int nInterval = 0)
{
	int nMillisecond = 6 * 1000;
    if (nMillisecond != 0 && nInterval > 1000)
    {
        while (true)
        {
            SendMDSPollDataRequest();

            std::this_thread::sleep_for(std::chrono::milliseconds(nMillisecond));

        }
    }
}


void SocketClient::SendMDSPollDataRequest()
{



}


void SocketClient::KeepConnectionAlive(int nInterval = 0)
{
	int nMillisecond = 6 * 1000;
    if (nMillisecond != 0 && nInterval > 1000)
    {
        while (true)
        {
            SendMDSCreateEventResult();

            std::this_thread::sleep_for(std::chrono::milliseconds(nMillisecond));

        }
    }
}


void SocketClient::SendMDSCreateEventResult()
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

		/// @todo add where nInterval is actually coming from
		int nInterval = 0;


		//Recheck MDS Attributes	//Task.Run(() => _MPudpclient.RecheckMDSAttributes(nInterval));				
		std::thread recheckMDSAttributesThread([&]() {	
			//RecheckMDSAttributes(nInterval);
			RecheckMDSAttributes(nInterval);

		});
		recheckMDSAttributesThread.detach(); //allows the thread to continue executing independently, without requiring synchronization or joining with the parent thread

				//lambda functions in C++:	[capture-list](parameters) -> return-type { function-body }
				//capture-list being the variables from the enclosing scope that you want to have accessible within the lambda function, can be captured by value, reference, or a mix of both

				//possibly [&] instead of [this] --> & indicates that you want to capture all variables from the enclosing scope by reference, 
				//this for capturing all the class members by ref so you can work w the class members within the lambda body as if you were inside a member function of the class


		//Keep Connection Alive		//Task.Run(() => _MPudpclient.KeepConnectionAlive(nInterval));
		std::thread keepConnectionAliveThread([&]() {
			//KeepConnectionAlive(nInterval);
			KeepConnectionAlive(nInterval);
		});
		keepConnectionAliveThread.detach();


		//Receive PollDataResponse message / Receive poll data		//_MPudpclient.BeginReceive(new AsyncCallback(ReceiveCallback), state);
		std::thread receiveThread([&]() {
			BeginReceive(); //receive data asynchronously
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

