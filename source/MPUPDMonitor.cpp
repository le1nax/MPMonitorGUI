// #include "../include/MPUDPMonitor.h"
// #include <iostream>



// // MPUPDMonitor::MPUPDMonitor(std::string ipAddress){


// 	// this->pathToFile = "temp";
// 	// Assign IP address
// 	// if(ipAddress != ""){
// 	// 	this->ipAddress = ipAddress;
// 	// }
// 	// else{
// 	// 	std::cerr << "Empty IP Address, " << ipAddress << std::endl;
// 	// }
// 	// Create UDPClient
// 	// this->client = UDPClient(this->ipAddress, this->port);
// 	// Send and Receive Association Request
// 	// this->Association();
// 	// Receive MDSCreateEventReport and Send MDSCreateEventResult
// 	// this->MDSCreateEvent();
// 	// Send Extended PollData Requests cycled every second
	
// // 	// Start running (PollRequest + Receive Data)
// // 	this->run();
// // }


// // void MPUPDMonitor::Association(){

// 	// Send AssociationRequest and receive the Association Result message
// 	//this->SendWaveAssociationRequest();
// 	// Receive MDSCreateEventReport
// 	// client->UDPReceive(this->recvBuf);
// 	// Send MDSCreateEvent Result message
// 	// this->ProcessPacket(recvBuf, client->recvResult); // recvResult is the length of the received Package
// //}




// void MPUPDMonitor::SendWaveAssociationRequest() {
// 	// // Send AssociationRequest
// 	// this->sendBuf = DataConstants::aarq_msg_wave_ext_poll2;
// 	// client->UDPRequest(sendBuf);
// 	// // Receive AssociationResult message
// 	// client->UDPReceive(this->recvBuf);
// 	// // Todo: Log message in log file
// }


// void MPUPDMonitor::ProcessPacket(uint8_t* packetbuffer, size_t len)
// {
// 	// std::istringstream memstream(std::string((char*)packetbuffer, len));
// 	// std::binary_reader binreader(memstream);

// 	// uint8_t sessionheader[4];
// 	// binreader.read((char*)sessionheader, 4); // Read fist 4 bytes from stream
// 	// uint16_t ROapdu_type = correctendianshortus(binreader.read<uint16_t>()); //read two bytes as uint16 and convert to correct endian

// 	// switch (ROapdu_type)
// 	// {
// 	// case DataConstants::ROIV_APDU:
// 	// 	// This is an MDS create event, answer with create response
// 	// 	ParseMDSCreateEventReport(packetbuffer);
// 	// 	this->SendMDSCreateEventResult();
// 	// 	break;
// 	// case DataConstants::RORS_APDU:
// 	// 	this->CheckPollPacketActionType(packetbuffer);
// 	// 	break;
// 	// case DataConstants::RORLS_APDU:
// 	// 	this->CheckLinkedPollPacketActionType(packetbuffer);
// 	// 	break;
// 	// case DataConstants::ROER_APDU:
// 	// 	break;
// 	// default:
// 	// 	break;
// 	// }
// }




// // void MPUPDMonitor::CheckPollPacketActionType(const uint8_t* packetbuffer, size_t len) {

// 	// std::stringstream memstream;
// 	// memstream.write((char*)packetbuffer, len);

// 	// uint8_t header[20];
// 	// memstream.read((char*)header, sizeof(header));
// 	// uint16_t action_type;
// 	// memstream.read((char*)&action_type, sizeof(action_type));
// 	// action_type = correctendianshortus(action_type);
// 	// m_actiontype = action_type;

// 	// switch (action_type)
// 	// {
// 	// case DataConstants::NOM_ACT_POLL_MDIB_DATA:
// 	// 	PollPacketDecoder(packetbuffer, 44);
// 	// 	break;
// 	// case DataConstants::NOM_ACT_POLL_MDIB_DATA_EXT:
// 	// 	PollPacketDecoder(packetbuffer, 46);
// 	// 	break;
// 	// default:
// 	// 	break;
// 	// }

// // }



// // void MPUPDMonitor::CheckLinkedPollPacketActionType(const uint8_t* packetbuffer, size_t len) {

// 	// std::stringstream memstream;
// 	// memstream.write((char*)packetbuffer, len);
// 	// memstream.seekg(0);

// 	// std::istream_iterator<unsigned char> start(memstream), end;
// 	// std::vector<unsigned char> buffer(start, end);

// 	// std::vector<unsigned char> header(buffer.begin(), buffer.begin() + 22);
// 	// unsigned short action_type = correctendianshortus(*(unsigned short*)(buffer.data() + 22));
// 	// m_actiontype = action_type;

// 	// switch (action_type)
// 	// {
// 	// case DataConstants::NOM_ACT_POLL_MDIB_DATA:
// 	// 	PollPacketDecoder(packetbuffer, 46);
// 	// 	break;
// 	// case DataConstants::NOM_ACT_POLL_MDIB_DATA_EXT:
// 	// 	PollPacketDecoder(packetbuffer, 48);
// 	// 	break;
// 	// default:
// 	// 	break;
// 	// }

// // }







// uint16_t MPUPDMonitor::correctendianshortus(uint16_t sValue) {
	
// 	// uint8_t bArray[sizeof(uint16_t)];
// 	// std::memcpy(bArray, &sValue, sizeof(uint16_t));
// 	// if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) std::reverse(bArray, bArray + sizeof(uint16_t));

// 	// uint16_t result;
// 	// std::memcpy(&result, bArray, sizeof(uint16_t));
// 	// return result;

//  }

// uint32_t MPUPDMonitor::correctendianuint(uint32_t sValue) {

// 	// uint8_t bArray[sizeof(sValue)];
// 	// memcpy(&bArray, &sValue, sizeof(sValue));
// 	// if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
// 	// 	std::reverse(bArray, bArray + sizeof(sValue));

// 	// uint32_t result;
// 	// memcpy(&result, &bArray, sizeof(result));
// 	// return result;

// }


// // /*
// // #include <chrono>
// // #include <thread>

// // void SendCycledExtendedPollWaveDataRequest(int nInterval) {
// // 	int nmillisecond = nInterval;

// // 	if (nmillisecond != 0) {
// // 		do {
// // 			MPClient.Send(DataConstants::ext_poll_request_wave_msg, DataConstants::ext_poll_request_wave_msg_length);
// // 			std::this_thread::sleep_for(std::chrono::milliseconds(nmillisecond));
// // 		} while (true);
// // 	} else {
// // 		MPClient.Send(DataConstants::ext_poll_request_wave_msg, DataConstants::ext_poll_request_wave_msg_length);
// // 	}
// // }
// // */
