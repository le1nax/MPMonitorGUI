#pragma once

#define RECV_BUFFER_SIZE (1380) // Maximum message size of Philips Monitor
#define SEND_BUFFER_SIZE () //
#include <list>
#include <string>
#include <vector>
#include <cstring>
#include <stdint.h>
#include <time.h>
#include "MPMonitor.h"



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




class MPUPDMonitor:  public MPMonitor{

public:
	MPUPDMonitor(std::string ipAddress);
	void getConnection();

	struct GlbHandle
    {
		
         unsigned short context_id;
         unsigned short handle;
    };

	 struct ManagedObjectId
    {
		
         unsigned short m_obj_class;
         GlbHandle m_obj_inst;
    };

	struct NumericValResult
	{
	std::string Timestamp;
	std::string Relativetimestamp;
	std::string SystemLocalTime;
	std::string PhysioID;
	std::string Value;
	std::string DeviceID;
	};

   struct ActionResult
    {
		
         ManagedObjectId objectid;
         unsigned short action_type;
         unsigned short length;

    };

	 struct ScaleRangeSpec16
    {
		
         double lower_absolute_value;
         double upper_absolute_value;
         unsigned short lower_scaled_value;
         unsigned short upper_scaled_value;
         unsigned short obpoll_handle;
         unsigned short physio_id;
    };

 	struct Ava
    {
		
         unsigned short attribute_id;
         unsigned short length;
         unsigned short attribute_val; //placeholder
    };

	 struct AttributeList
    {
		
         unsigned short count;
         unsigned short length;
        // Ava [] value = new Ava[1];
         Ava value1; //null placeholder
         std::vector<std::byte> avaobjectsarray;
    };

	 struct SessionHeader
    {
		
         std::byte type;
         std::byte length;
         unsigned short length2;
    };

	struct ObservationPoll
    {
		
         unsigned short obj_handle;
         AttributeList attributes;
         std::vector<std::byte> avaobjectsarray;
    };

    struct SingleContextPoll
    {
		unsigned short context_id;
        unsigned short count;
        unsigned short length;
        // ObservationPoll [] value = new ObservationPoll[1];
        ObservationPoll value1; //null placeholder
        std::vector<std::byte> obpollobjectsarray;
    };
    struct PollInfoList
    {
		unsigned short count;
        unsigned short length;
        // SingleContextPoll [] value= new SingleContextPoll[1];
        SingleContextPoll value1; //null placeholder
        std::vector<std::byte> scpollarray;
    };

	struct SaSpec
    {
		
         unsigned short array_size;
         std::byte sample_size;
         std::byte significant_bits;
         unsigned short SaFlags;
         unsigned short obpoll_handle;
    };

	struct SaCalibData16
    {
		
         double lower_absolute_value;
         double upper_absolute_value;
         unsigned short lower_scaled_value;
         unsigned short upper_scaled_value;
         unsigned short increment;
         unsigned short cal_type;
         unsigned short obpoll_handle;
         unsigned short physio_id;
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


    struct RawFrameHdr
    {
		
         std::byte protocol_id;
         std::byte msg_type;
         unsigned short length;
    };

     struct ROapdus
    {
		
         unsigned short ro_type;
         unsigned short length;
    };

     struct RORSapdu
    {
		
         unsigned short invoke_id;
         unsigned short command_type;
         unsigned short length;
    };

     struct ROIVapdu
    {
		
         unsigned short inovke_id;
         unsigned short command_type;
         unsigned short length;
    };

     struct RORLSapdu
    {
		
         std::byte state;
         std::byte count;
         RORSapdu apdu;
    };




     struct AbsoluteTime
    {
		
         std::byte century;
         std::byte year;
         std::byte month;
         std::byte day;
         std::byte hour;
         std::byte minute;
         std::byte second;
         std::byte fraction;
    };

     struct ObjectType
    {
		
         unsigned short partition;
         unsigned short code;
    };

     struct PollMdibDataReplyExt
    {
		
         unsigned short poll_number;
         unsigned short sequence_no;
         unsigned int rel_time_stamp;
         AbsoluteTime abs_time_stamp;
         ObjectType type;
         unsigned short polled_attr_grp;
    };

     struct ReceiveState
    {
		
         unsigned int state;
         unsigned int position;
    };

     struct NuObsValue
    {
		
         unsigned short physio_id;
         unsigned short state;
         unsigned short unit_code;
         unsigned int value;
    };

     struct NuObsValueCmp
    {
		
         unsigned short count;
         unsigned short length;
        // NuObsValue [] value = new NuObsValue[1];
         NuObsValue value1;
    };

     struct SaObsValue
    {
		
         unsigned short physio_id;
         unsigned short state;
         unsigned short length;
        // byte [] value = new byte[1];  
         std::byte value1;
    };

     struct SaObsValueCmp
    {
		
         unsigned short count;
         unsigned short length;
        // SaObsValue [] value = new SaObsValue[1];
         SaObsValue value1;
    };

    

    struct MetricStructure
    {
		
         std::byte ms_struct;
         std::byte ms_comp_no;
    };

    struct MetricSpec
    {
		
         unsigned int update_period;
         unsigned short category;
         unsigned short access;
        MetricStructure structure;
         unsigned short relevance;
    };

    struct StringMP
    {
		
         unsigned short length;
        // unsigned short [] value = new unsigned short[1];
         unsigned short value1;
    };

    struct PollProfileExt
    {
		
         unsigned int options;
        AttributeList ext_attr;
    };

    struct PollProfileSupport
    {
		
         unsigned int poll_profile_revision;
         unsigned int min_poll_period;
         unsigned int max_mtu_rx;
         unsigned int max_mtu_tx;
         unsigned int max_bw_tx;
         unsigned int options;
        AttributeList optional_packages;
    };

    
    struct MdseUserInfoStd
    {
		
         unsigned int protocol_version;
         unsigned int nomenclature_version;
         unsigned int functional_units;
         unsigned int system_type;
         unsigned int startup_mode;
         unsigned int option_list;
        AttributeList supported_aprofiles;
    };

     struct EventReportArgument
    {
		
         ManagedObjectId managed_object;
         unsigned int relative_time;
         unsigned short event_type;
         unsigned short length;

    };

    struct MDSCreateEvenReport
    {
		
         SessionHeader session_hdr;
         ROapdus remoteop_hdr;
         ROIVapdu remoteinvoke_hdr;
         EventReportArgument eventreport_hdr;
         ManagedObjectId objectid;
         AttributeList attributes;

    };

    //-----------------------------------------------------------------------------


     struct NumericObject
    {
		
         unsigned int validity;
         unsigned short pollNumber;
         unsigned int label;
        NuObsValue value;
        MetricSpec spec;
        StringMP labelString;

    };


    struct SinglePollLinkedPacketResult
    {
		
         SessionHeader session_hdr;
         ROapdus remoteop_hdr;
         RORLSapdu remoteop_cmd;
         ActionResult action_result;
         //PollMdibDataReply mdib_data;
         PollInfoList pollinfo_list; //null placeholder

    };

    struct ExtPollLinkedPacketResult
    {
		
         SessionHeader session_hdr;
         ROapdus remoteop_hdr;
         RORLSapdu remoteop_cmd;
         ActionResult action_result;
         PollMdibDataReplyExt mdib_data;
         PollInfoList pollinfo_list; //null placeholder

    };
	struct SinglePollPacketResult
    {
		
         SessionHeader session_hdr;
         ROapdus remoteop_hdr;
         RORSapdu remoteop_cmd;
         ActionResult action_result;
         //PollMdibDataReply mdib_data;
         PollInfoList pollinfo_list; //null placeholder

    };




	struct IDLabel
	{
		uint32_t idlabelhandle;
		std::string idlabelstring;
		uint16_t obpoll_handle;
	};

	std::vector<NumericValResult> m_NumericValList;
	std::vector<std::string> m_NumValHeaders;
	std::string m_strbuildvalues;
	std::string m_strbuildheaders;
	std::vector<WaveValResult> m_WaveValResultList;
	std::string m_strbuildwavevalues;
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
	tm m_baseDateTime = { 0 };
	tm m_pollDateTime = { 0 };
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
	tm GetAbsoluteTimeFromBCDFormat(const uint8_t* bcdtimebuffer);
	void GetBaselineRelativeTimestamp(const uint8_t* timebuffer);
	void ProcessPacket(const uint8_t* packetbuffer, size_t len);
	void CheckPollPacketActionType(const uint8_t* packetbuffer, size_t len);
	void CheckLinkedPollPacketActionType(const uint8_t* packetbuffer, size_t len);
	void PollPacketDecoder(const uint8_t* packetbuffer, int headersize);
	void ProcessPacket(uint8_t* packetbuffer, size_t len);
	int DecodePollObjects(PollInfoList pollobjects, std::vector<std::byte> packetbuffer);
	int DecodeSingleContextPollObjects(SingleContextPoll scpoll);
	int DecodeObservationPollObjects(ObservationPoll obpollobject);
	void DecodeAvaObjects(Ava avaobject);
	std::string GetPacketTimestamp(const uint8_t* header);
	void ReadIDHandle(const uint8_t* avaattribobjects);
	void ReadIDLabel(const uint8_t* avaattribobjects);
	void ReadIDLabelString(const uint8_t* avaattribobjects);
	void AddIDLabelToList();
	void ReadNumericObservationValue(const uint8_t* avaattribobjects);
	void ReadCompoundNumericObsValue(const uint8_t* avaattribobjects);
	void ReadWaveSaObservationValueObject(const uint8_t* avaattribobjects);
	void ReadSaSpecifications(const uint8_t* avaattribobjects);
	void ReadWaveSaObservationValue();
	void ReadCompoundWaveSaObservationValue(const uint8_t* avaattribobjects);
	void ReadSaScaleSpecifications(const uint8_t* avaattribobjects);
	void ReadSaCalibrationSpecifications(const uint8_t* avaattribobjects);
	static double FloattypeToValue(uint32_t fvalue);
	static uint16_t Get16bitLSBfromunsigned(uint32_t sourcevalue);
	//static int correctendianshort(uint16_t sValue); Not used
	static uint16_t correctendianshortus(uint16_t sValue); // Is used
	static uint32_t correctendianunsigned(uint32_t sValue); // Is used
	//static int16_t correctendianshorts(int16_t sValue); Not used
	uint32_t correctendianuint(uint32_t sValue);
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
	void ExportNumValListToCSVFile(std::string _FileName, std::string strbuildNumVal);
	bool ByteArrayToFile(std::string _FileName, uint8_t* _ByteArray, int nWriteLength);



private:
	bool connected = false;
	std::string ipAddress;
	std::string pathToFile;
	const int port = 24105;
	bool startRecord = false;
	std::vector<char> recvBuf;
	std::vector<char> sendBuf;
	// UDPClient client;
	static int BinaryCodedDecimalToInteger(int value);
	void getData();
	void Association();
	void MDSCreateEvent();
	void run();
	
};