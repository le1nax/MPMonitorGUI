#pragma once

#define RECV_BUFFER_SIZE (1380) // Maximum message size of Philips Monitor
#define SEND_BUFFER_SIZE () //
#include <list>
#include <string>
#include <vector>
#include <cstring>
#include <stdint.h>


//TODO Make sure the BootP/DHCP server gives an IP address to the monitor --> how to find out the correct IP address?
//TODO AssociationRequest --> What is the message and how to send it? Are there several messages to send?
//TODO MDSCreateEventRequest--> What does it do and how do I do it?
//TODO inspection of results
//TODO Polling of waves-forms and numerics --> is it possible to make it dynamic according to user input?
//TODO How to save data (ring buffer needed?)

/**
*	Class for reading data of a Philips Patientmonitor (tested for MX700) via LAN connection
*	A winsock is created and used for connection using UDP/IP
*	The IP of the patient monitor can be set manually in service mode/same holds for the computer
*	The connetction protocol is defined at 
*	https://www.documents.philips.com/doclib/enc/fetch/2000/4504/577242/577243/577247/582636/582882/X2%2C_MP%2C_MX_&_FM_Series_Rel._L.0_Data_Export_Interface_Program._Guide_4535_645_88011_(ENG).pdf
* 
**/


struct NumericValResult
{
	std::string Timestamp;
	std::string Relativetimestamp;
	std::string SystemLocalTime;
	std::string PhysioID;
	std::string Value;
	std::string DeviceID;
};
struct  WaveValResult
{
	std::string Timestamp;
	std::string Relativetimestamp;
	std::string SystemLocalTime;
	std::string PhysioID;
	uint8_t Value;
	std::string DeviceID;
	uint16_t obpoll_handle;
	SaSpec saSpecData;
	SaCalibData16 saCalibData;
	ScaleRangeSpec16 ScaleRangeSpec16;
};


struct IDLabel
{
	uint32_t idlabelhandle;
	std::string idlabelstring;
	uint16_t obpoll_handle;
};




class MPUPDMonitor: public MPMonitor{

public:
	MPUPDMonitor(std::string ipAddress);
	void getConnection();



	std::vector<NumericValResult> m_NumericValList;
	std::vector<std::string> m_NumValHeaders;
	std::stringstream m_strbuildvalues;
	std::stringstream m_strbuildheaders;
	std::vector<WaveValResult> m_WaveValResultList;
	std::stringstream m_strbuildwavevalues;
	bool m_transmissionstart = true;
	std::string m_strTimestamp;
	uint16_t m_actiontype;
	int m_elementcount = 0;
	int m_headerelementcount = 0;
	int m_csvexportset = 1;
	std::vector<SaSpec> m_SaSpecList;
	std::vector<SaCalibData16> m_SaCalibDataSpecList;
	std::vector<ScaleRangeSpec16> m_ScaleRangeSpecList;
	std::vector<IDLabel> m_IDLabelList;
	bool m_calibratewavevalues = false;

	uint16_t m_obpollhandle = 0;
	uint32_t m_idlabelhandle = 0;
	std::string m_idlabelstring;
	SYSTEMTIME m_baseDateTime = { 0 };
	SYSTEMTIME m_pollDateTime = { 0 };
	uint32_t m_baseRelativeTime = 0;
	std::string m_DeviceID;
	std::string m_jsonposturl;
	int m_dataexportset = 1;

	

	//void SendAssociationRequest(); // not used at the moment
	void SendWaveAssociationRequest();
	void SendMDSCreateEventResult();
	void SendMDSPollDataRequest();
	void SendPollDataRequest();
	void SendExtendedPollDataRequest();
	void SendExtendedPollWaveDataRequest();
	void GetRTSAPriorityListRequest();
	void SetRTSAPriorityListRequest();
	void SetRTSAPriorityList(int nWaveSetType);
	static void CreateWaveformSet(int nWaveSetType, std::vector<uint8_t> WaveTrtype);
	void SendRTSAPriorityMessage(const uint8_t* WaveTrType);
	SYSTEMTIME GetAbsoluteTimeFromBCDFormat(const uint8_t* bcdtimebuffer);
	void GetBaselineRelativeTimestamp(const uint8_t* timebuffer);
	void ProcessPacket(const uint8_t* packetbuffer, size_t len);
	void CheckPollPacketActionType(const uint8_t* packetbuffer, size_t len);
	void CheckLinkedPollPacketActionType(const uint8_t* packetbuffer, size_t len);
	void PollPacketDecoder(const uint8_t* packetbuffer, int headersize);
	int DecodePollObjects(PollInfoList pollobjects, byte[] packetbuffer);
	int DecodeSingleContextPollObjects(SingleContextPoll scpoll, std::binary_reader binreader2);
	int DecodeObservationPollObjects(ObservationPoll obpollobject, std::binary_reader binreader3);
	void DecodeAvaObjects(Ava avaobject, std::binary_reader binreader4);
	std::string GetPacketTimestamp(const uint8_t* header);
	void ReadIDHandle(const uint8_t* avaattribobjects);
	void ReadIDLabel(const uint8_t* avaattribobjects);
	void ReadIDLabelString(const uint8_t* avaattribobjects);
	void AddIDLabelToList();
	void ReadNumericObservationValue(const uint8_t* avaattribobjects);
	void ReadCompoundNumericObsValue(const uint8_t* avaattribobjects);
	void ReadWaveSaObservationValueObject(const uint8_t* avaattribobjects);
	void ReadSaSpecifications(const uint8_t* avaattribobjects);
	void ReadWaveSaObservationValue(std::binary_reader binreader7);
	void ReadCompoundWaveSaObservationValue(const uint8_t* avaattribobjects);
	void ReadSaScaleSpecifications(const uint8_t* avaattribobjects);
	void ReadSaCalibrationSpecifications(const uint8_t* avaattribobjects);
	static double FloattypeToValue(uint32_t fvalue);
	static uint16_t Get16bitLSBfromUInt(uint32_t sourcevalue);
	//static int correctendianshort(uint16_t sValue); Not used
	static uint16_t correctendianshortus(uint16_t sValue); // Is used
	static uint32_t correctendianuint(uint32_t sValue); // Is used
	//static int16_t correctendianshorts(int16_t sValue); Not used
	void ExportDataToCSV();
	void WriteNumericHeadersList();
	void SaveNumericValueList();
	void SaveNumericValueListRows();
	void SaveNumericValueListConsolidatedCSV();
	void WriteNumericHeadersListConsolidatedCSV();
	void ExportWaveToCSV();
	static int CreateMask(int significantbits);
	double CalibrateSaValue(double Waveval, SaCalibData16 sacalibdata);
	double ScaleRangeSaValue(double Waveval, ScaleRangeSpec16 sascaledata);
	void ExportNumValListToCSVFile(std::string _FileName, std::stringstream strbuildNumVal);
	bool ByteArrayToFile(std::string _FileName, uint8_t* _ByteArray, int nWriteLength)



private:
	bool connected = false;
	std::string ipAddress;
	std::string pathToFile;
	const int port = 24105;
	bool startRecord = false;
	char[RECV_BUFFER_SIZE] recvBuf;
	char[SEND_BUFFER_SIZE] sendBuf;
	UDPClient client;
	static int BinaryCodedDecimalToInteger(int value);
	void getData();
	void Association();
	void MDSCreateEvent();
	void run();
	
};