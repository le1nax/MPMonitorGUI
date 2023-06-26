#include "../include/SocketClient.h"
#include "../include/definitions.h"
#include "../include/AvaObj.h"
#include <iostream>

using namespace std;

/// @todo Konstruktor soll die membervariablen uA ipRemoteTarget initialisieren
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
    sendBytes(aarq_msg_wave_ext_poll2);
}

void SocketClient::ProcessPacket(char* buffer)
{
    //size_t session_head_size = sizeof(4bytes)
    size_t session_head_size = 4;
    size_t ROapdu_type_size = 2;
	auto ROapdu_type = htons(ReadByteValuesFromBuffer(buffer, sizeof(buffer),session_head_size, ROapdu_type_size));
    switch (ROapdu_type)
            {
                case ROIV_APDU:
                    // This is an MDS create event, answer with create response
                    ParseMDSCreateEventReport(buffer);
                    SendMDSCreateEventResult();
                    break;
                case RORS_APDU:
                    CheckPollPacketActionType(buffer);
                    break;
                case RORLS_APDU:
                    CheckLinkedPollPacketActionType(buffer);
                    break;
                case ROER_APDU:
                    break;
                default:
                    break;
            }
}

void SocketClient::CheckLinkedPollPacketActionType(char* buffer)
{

}

void SocketClient::ParseMDSCreateEventReport(char* buffer)
{
    //size_t header_size = sizeof(34bytes)
    size_t attriblist_count_size = 2;
    size_t attriblist_length_size = 2;
    size_t header_size = 34;
    auto attriblist_count = htons(ReadByteValuesFromBuffer(buffer, sizeof(buffer),header_size, attriblist_count_size));
    auto attriblist_length = htons(ReadByteValuesFromBuffer(buffer, sizeof(buffer),header_size+attriblist_count_size, attriblist_length_size));
    int avaobjectscount = attriblist_count;   
     if (avaobjectscount > 0)
            {
                char* attriblistobjects_buffer = ReadBytesFromBuffer(buffer, sizeof(buffer),header_size+attriblist_count_size+attriblist_length_size, attriblist_length);

                for (size_t i = 0; i < avaobjectscount; i++)
                {
                    DecodeMDSAttribObjects(move(make_unique<AvaObj>()), attriblistobjects_buffer);
                }
            }

}

void SocketClient::DecodeMDSAttribObjects(unique_ptr<AvaObj> avaobject, char* attriblistobjects_buffer, size_t count)
{

    ///@todo sizeof 2 bytes per count
    avaobject->attribute_id = htons(ReadByteValuesFromBuffer(attriblistobjects_buffer, sizeof(attriblistobjects_buffer), 4*count, 2)); 
    avaobject->length = htons(ReadByteValuesFromBuffer(attriblistobjects_buffer, sizeof(attriblistobjects_buffer), 4*count+2, 2)); 
     //avaobject.attribute_val = correctendianshortus(binreader4.ReadUInt16());
    if (avaobject->length > 0)
        {
            char* avaattribobjects_buffer = ReadBytesFromBuffer(attriblistobjects_buffer, sizeof(attriblistobjects_buffer), 4*(count+1), avaobject->length);


            switch (avaobject->attribute_id)
             {
                //Get Date and Time
                case NOM_ATTR_TIME_ABS:
                    m_baseDateTime = GetAbsoluteTimeFromBCDFormat(avaattribobjects_buffer);
                        break;
                    //Get Relative Time attribute
                    case NOM_ATTR_TIME_REL:
                        GetBaselineRelativeTimestamp(avaattribobjects_buffer);
                        break;
                    //Get Patient demographics
                    case NOM_ATTR_PT_ID:
                        break;
                    case NOM_ATTR_PT_NAME_GIVEN:
                        break;
                    case NOM_ATTR_PT_NAME_FAMILY:
                        break;
                    case NOM_ATTR_PT_DOB:
                        break;
            }
    }
}

void SocketClient::GetBaselineRelativeTimestamp(char* bcdtimebuffer)
{
    m_baseRelativeTime = htons(int(ReadByteValuesFromBuffer(bcdtimebuffer, sizeof(bcdtimebuffer), 0, sizeof(bcdtimebuffer))));
}

tm SocketClient::GetAbsoluteTimeFromBCDFormat(char* bcdtimebuffer)
        {
            ///@todo vielleicht anderen struct als tm benutzen
            tm dateTime = m_baseDateTime;

            int century = BinaryCodedDecimalToInteger(bcdtimebuffer[0]); //century
            int year = BinaryCodedDecimalToInteger(bcdtimebuffer[1]); //year
            int month = BinaryCodedDecimalToInteger(bcdtimebuffer[2]); //month
            int day = BinaryCodedDecimalToInteger(bcdtimebuffer[3]);//day
            int hour = BinaryCodedDecimalToInteger(bcdtimebuffer[4]);//hour
            int minute = BinaryCodedDecimalToInteger(bcdtimebuffer[5]);//minute
            int second = BinaryCodedDecimalToInteger(bcdtimebuffer[6]);//second
            int fraction = BinaryCodedDecimalToInteger(bcdtimebuffer[7]); //fraction

            int formattedyear = (century * 100) + year;

            dateTime.tm_sec = second;
            dateTime.tm_min = minute;
            dateTime.tm_hour = hour;
            dateTime.tm_mday = day;
            dateTime.tm_mon = month;
            dateTime.tm_year = year;

            return dateTime;

        }
    


void SocketClient::SendMDSCreateEventResult()
{

}

void SocketClient::CheckPollPacketActionType(char* buffer)
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
            //Receive AssociationResult message 
			///@todo schauen wie viel Buffer space wir brauchen
            char readassocbuffer[1000] = "";
            sockaddr_in addr;
					//try:
            addr = RecvFrom(readassocbuffer, 200);
                    // _MPudpclient.ByteArrayToFile(path, readassocbuffer, readassocbuffer.GetLength(0));
                    //Console.WriteLine("Receiving SOC buffer");
					//if(addr = "")
					//{cout << "address empty">> endl;}		
					// catch: 
			//Receive MDSCreateEventReport message
            char readmdsconnectbuffer[200] = "";
            sockaddr_in addr;
            addr = RecvFrom(readmdsconnectbuffer, 200);
            		//readmdsconnectbuffer = _MPudpclient.Receive(ref _MPudpclient.m_remoteIPtarget);
                    // _MPudpclient.ByteArrayToFile(path, readmdsconnectbuffer, readmdsconnectbuffer.GetLength(0));
                    // //Console.WriteLine("Receiving ARRAY from buffer");

            //Send MDSCreateEventResult message
            ProcessPacket(readmdsconnectbuffer);

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
        
//         string data = "hello world"; //define an example string to send
//         char buffer[100]; //create a buffer to hold received data

//         Socket.SendTo("127.0.0.1", 100, data.c_str(), data.size()); // Send data to IP address and port 100
//         Socket.RecvFrom(buffer, 100); // Receive data of length 100 into the buffer
//         cout << buffer; // Output the received data
//     }
//     catch (exception &ex) //catch any occurring system errors
//     {
//         cout << ex.what();  //print error message
//     }
//     char c;
//     cin >> c; // Wait for user input before exiting
//}
}

