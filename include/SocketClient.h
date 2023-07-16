#pragma once
/// SocketClient is the Client class
///
/// It processes the protocol dialog between Monitor and Client
#include "Socket.h"


#define RECEIVEINTERVAL 500 // koennen den receive thread nicht komplett durchlaufen lassen weil wir keinen echten Parallelismus haben


///@brief The SocketClient class is derived from the UDPSocket class and implements the UDP client connecting to the patient monitor and processing the received data
class SocketClient : public UDPSocket {

    public:
        /// @brief Constructor of class SocketClient
        /// @param remoteIPtarget the IP address to send to
        /// @param remotePortTarget the port of the target address
        /// @todo Konstruktor soll die membervariablen initialisieren
        SocketClient(std::string remoteIPtarget, const unsigned short remotePortTarget);
        ~SocketClient() = default;
        
        /// @brief A method to establish a LAN connection to the monitor, keep the connection alive and exchange data
        void establishLanConnection();

        /// @brief a getter method for the SOCKET sock
        /// @return the membervariable SOCKET sock
        SOCKET getSocket();

        /// @brief Sendet Assiation Request
        /// @brief A method that sends a Wave Association Request by sending the corresponding bytes via sendBytes()
        void SendWaveAssociationRequest();
        
        sockaddr_in m_sa_remoteIPtarget;
        /// @brief A method for receiving data from the server
        /// @param buffer1 the buffer that contains the data
        /// @param buffersize the size of the buffer
        /// @param flags the flags to set for the base WSARecvFrom function
        void Receive(char* buffer, size_t buffersize = maxbuffersize, int flags = 0);
    private:
        /// @brief A method to send the specified bytes to the target address by putting them in a buffer and sending them via SendTO()
        /// @param bytes the bytes to be sent 
        void sendBytes(std::vector<std::byte> bytes);

        /// @brief Processes all incoming types of data protocols
        /// @param buffer char array with the sent bytes
        /// @brief the method that is called whenever data is received from the monitor to categorise, decode and process the received data
        /// @param buffer the buffer with the received data
        void ProcessPacket(char* buffer);

        char* m_lastMsg;
        size_t lastMsgSize = 0;

        std::vector<std::byte> m_readassocbuffer;
        std::vector<std::byte> m_readmdsconnectbuffer;
        std::string m_remoteIPtarget = "";
        std::string m_strTimestamp = "";
        std::string m_DeviceID= "";
        std::string m_idlabelstring = "";
        std::vector<NumericValResult> m_NumericValList;
        std::vector<std::string> m_NumValHeaders;
        const unsigned short m_port = 0; 
        uint32_t m_baseRelativeTime= 0;
        uint16_t m_actiontype= 0;
        uint16_t m_obpollhandle = 0;
        uint32_t m_idlabelhandle = 0;
        uint32_t m_dataexportset = 1;
        uint32_t m_csvexportset = 1;
        std::vector<IDLabel> m_IDLabelList;
        std::vector<SaCalibData16> m_SaCalibDataSpecList;
        std::vector<ScaleRangeSpec16> m_ScaleRangeSpecList;
        std::vector<SaSpec> m_SaSpecList;
        std::vector<WaveValResult> m_WaveValResultList;
        /// @brief A method to create a mask for only the significant bits, here mainly for creating a mask for the msb
        /// @param significantbits the significant bits
        /// @return the mask
        int CreateMask(int significantbits);
        /// @brief A method to convert int to unsigned char
        /// @param value the int value to be converted
        /// @return the unsgined char
        unsigned char ConvertToByte(int value);
        
        /// @brief A method to check if data is in little endian format
        /// @return true if it is little endian
        bool IsLittleEndian();
        bool m_calibratewavevalues;
        /// @brief A method to calibrate the received wave values according to SaCalibData16
        /// @param Waveval the wave value to be calibrated
        /// @param sacalibdata the calibration parameters
        /// @return the calibrated Wave value
        double CalibrateSaValue(double Waveval, SaCalibData16 sacalibdata);
        /// @brief A method to scale the received wave values according to ScaleRangeSpec16
        /// @param Waveval the wave value to be scaled
        /// @param sascaledata the scaling parameters
        /// @return the scaled wave value
        double ScaleRangeSaValue(double Waveval, ScaleRangeSpec16 sascaledata);
        /// @brief A method to export numeric and wave data to the csv file
        /// @param filePath the file path to the csv file
        /// @param data the data to be exported
        void ExportNumValListToCSVFile(const std::string& filePath, const std::string& data);
        /// @brief A method for adding to the CSVBuilder for exporting data to a csv file
        /// @param csvBuilder the CSVBuilder
        /// @param timestamp the timestamp to be exported
        /// @param relativetimestamp the relativetimestamp to be exported
        /// @param systemLocalTime the systemLocalTime to be exported
        /// @param waveval the wavevalue to be exported
        void AppendToCSVBuilder(std::ostringstream& csvBuilder, const std::string& timestamp, const std::string& relativetimestamp, const std::string& systemLocalTime, double waveval);        tm m_baseDateTime{};
        void SaveNumericValueListRows();
        /// @brief A method to write the headers for the csv file of the exported data
        void WriteNumericHeadersList();
        /// @brief A method to write the numeric values for the csv file of the exported data
        void SaveNumericValueListConsolidatedCSV();
        /// @brief A method to write the headers for the csv file of the exported data at transmission start
        void WriteNumericHeadersListConsolidatedCSV();
        bool m_transmissionstart = TRUE;
        tm GetAbsoluteTimeFromBCDFormat(char* bcdtimebuffer);
        void AddTmMillseconds(tm& time, double millisecs);

        /// @brief A method to initialise the possible messages with their corresponding bytes
        void initMsgs();
        public:
        /// 
        std::vector<std::byte> aarq_ms;
        std::vector<std::byte> aarq_msg_ext_poll;
        std::vector<std::byte> aarq_msg_ext_poll2;
        std::vector<std::byte> aarq_msg_wave_ext_poll;
        std::vector<std::byte> aarq_msg_wave_ext_poll2;
        std::vector<std::byte> mds_create_resp_msg;
        std::vector<std::byte> poll_mds_request_msg;
        std::vector<std::byte> poll_request_msg;
        std::vector<std::byte> poll_request_msg2;
        std::vector<std::byte> poll_request_msg3;
        std::vector<std::byte> poll_request_msg4;
        std::vector<std::byte> ext_poll_request_msg3;
        std::vector<std::byte> ext_poll_request_msg;
        std::vector<std::byte> ext_poll_request_msg2;
        std::vector<std::byte> ext_poll_request_msg4;
        std::vector<std::byte> ext_poll_request_msg5;
        std::vector<std::byte> ext_poll_request_wave_msg3;
        std::vector<std::byte> ext_poll_request_wave_msg;
        std::vector<std::byte> ext_poll_request_wave_msg2;
        std::vector<std::byte> ext_poll_request_alert_msg;
        std::vector<std::byte> get_rtsa_prio_msg;
        std::vector<std::byte> keep_alive_msg;
        std::vector<std::byte> set_rtsa_prio_msg;
        std::vector<std::byte> keep_alive_msg2;
        std::vector<std::byte> rlrq_msg;
        std::vector<std::byte> rlrq_resp_msg;
        std::vector<std::byte> assoc_abort_resp_msg;
        private:

        /// @brief A method that is called as part of the ProcessPacket() function to further categorise, decode and process the received data
        /// @param buffer the buffer with the receieved data
        void CheckLinkedPollPacketActionType(char* buffer);

        /// @brief If a packet is identified as a PollObject, it is decoded in this function
        /// @param pollobjects List with PollObjects to write on
        /// @param packetbuffer Buffer, that contains the Pollobj
        /// @return returns the amount of PollObjects in the list
        int DecodePollObjects(PollInfoList* pollobjects, char* packetbuffer);
        /// @brief If a packet is identified as a SingleContextPollObject, it is decoded in this function
        /// @param scpoll Struct to write on
        /// @param buffer Buffer, that contains the SingleContextPoll
        /// @return returns the amount of PollObjects in the SingleContextPoll
        int DecodeSingleContextPollObjects(SingleContextPoll* scpoll, char* buffer);
        /// @brief If a packet is identified as a SingleContextPollObject, it is decoded in this function
        /// @param scpoll ObservationPoll to write on 
        /// @param buffer Buffer, that contains the ObservationPoll
        /// @return returns the amount of PollObjects in the ObservationPoll
        int DecodeObservationPollObjects(ObservationPoll* scpoll, char* buffer);

        /// @brief Reads the IDLabel of a BufferObject
        /// @param buffer The Buffer to read from 
        void ReadIDLabel(char* buffer);
        /// @brief A method to read numeric observation values from the buffer
        /// @param buffer buffer to read from
        void ReadNumericObservationValue(char* buffer);
        /// @brief A method to read compound numeric observation values from the buffer
        /// @param buffer buffer to read from
        void ReadCompoundNumericObsValue(char* buffer);
        /// @brief Reads the IDLabel of a BufferObject and adds it to a list.
        /// @link AddIDLabelToList
        /// @param buffer buffer to read from 
        void ReadIDLabelString(char* buffer);
        /// @brief A method to read numerical observation values from the buffer
        /// @param buffer The Buffer to read from 
        void ReadWaveSaObservationValueObject(char* buffer);
        /// @brief A method to read wave observation values from the buffer
        /// @param buffer The Buffer to read from 
        void ReadCompoundWaveSaObservationValue(char* buffer);
        /// @brief Reads the SaSpecification from a given buffer
        /// @param buffer ReadWaveSaObservationValue
        void ReadSaSpecifications(char* buffer);
        /// @brief A method to read the Scaling Specifications from the buffer and set them for scaling later
        /// @param buffer The Buffer to read from 
        void ReadSaScaleSpecifications(char* buffer);
        /// @brief A method to read the Calibration Specifications from the buffer and set them for calibrating later
        /// @param buffer buffer to read from
        void ReadSaCalibrationSpecifications(char* buffer);
        /// @brief This method reads the WaveSaObservation Values from a given buffer
        /// @param buffer The Buffer to read from 
        void ReadWaveSaObservationValue(char* buffer);
        
        /// @brief This function gets the Timestamp from which the measurement was taken
        /// @param header The buffer to read from 
        /// @param headersize the size of the header to read from
        /// @return returns the string with the timestamp
        std::string GetPacketTimestamp(char* header, uint16_t headersize);
        /// @brief Converts a given relative timestamp into tm format
        /// @param currentRelativeTime the relative timestamp in uint32_t
        /// @return returns the absolut timestamp in tm format
        tm GetAbsoluteTimeFromRelativeTimestamp(uint32_t currentRelativeTime);
        /// @brief Decodes the AvaObjects. This includes all observation values.
        /// @param avaObj An AvaObject to write on
        /// @param buffer The Buffer to read from 
        void DecodeAvaObjects(std::unique_ptr<AvaObj> avaObj, char* buffer);
        /// @brief Wrapper function to process an identified PollPacket
        /// @param packetbuffer the given buffer to read from
        /// @param headersize the headersize, which is used to determine from which bufferindex to read from
        void PollPacketDecoder(char* packetbuffer, size_t headersize);
       
        ///@brief A method that is called as part of the ProcessPacket() function to further categorise, decode and process the received data
        ///@param buffer the buffer with the receieved data
        void ParseMDSCreateEventReport(char* buffer);
        /// @brief gets the realtive timestamp from a given buffer
        /// @param timebuffer the buffer to get the timestamp from
        void GetBaselineRelativeTimestamp(char* timebuffer);
        /// @brief Decodes a buffer which was identified to hold MDSAttributes
        /// @param avaobj a unique_ptr to an AvaObj to write on
        /// @param attriblistobjects_buffer The Buffer to read from 
        /// @param count the amount of Ava Objects
        void DecodeMDSAttribObjects(std::unique_ptr<AvaObj> avaobj,  char* attriblistobjects_buffer, size_t count);
        /// @brief A method for sending an MDSCreateEvent message
        void SendMDSCreateEventResult();
        /// @brief Identifies the ActionType from a given PollPacket
        /// @param buffer The Buffer to read from 
        void CheckPollPacketActionType(char* buffer);
        /// @brief sends the RTSAPriorityMessage 
        /// @param WaveTrType a byte vector with the bytes to be send
        void SendRTSAPriorityMessage(std::vector<std::byte> WaveTrType);
        /// @brief A method for the thread that repeatedly sends an extended poll data request
        /// @param nInterval the time interval between polls
        void SendCycledExtendedPollDataRequest(size_t nInterval);
        /// @brief A method for the thread that repeatedly sends an extended poll wave data request
        /// @param nInterval the time interval between polls
        void SendCycledExtendedPollWaveDataRequest(size_t nInterval);

        /// @brief A method to export the data to a csv file in different formats depending on the value of m_csvexportset
        void ExportDataToCSV();
        /// @brief A method to export wave data to a csv file
        void ExportWaveToCSV();
        /// @brief Determines the WaveformSet to be extracted, based on the user inputs
        /// @param nWaveSetType The wavetype to be written on
        /// @param WaveTrtype Userinput which kind of WaveSet should be extracted
        void CreateWaveformSet(size_t nWaveSetType, std::vector<std::byte> WaveTrtype);
        
        /// @brief A method for sending a request to receive an RTSA priority list
        void GetRTSAPriorityListRequest();
        /// @brief sets the RTSA Priority List based on the user input
        /// @param nWaveSetType The userinput which wavetype should be set
        void SetRTSAPriorityList(size_t nWaveSetType);
        /// @brief one case of ExportDataToCSV which is to export the data to a csv file in different formats depending on the value of m_csvexportset
        void SaveNumericValueList();
        /// @brief adds the IDLabel to the m_obpollhandle
        void AddIDLabelToList();
        /// @brief A method for repeatedly rechecking the monitors MDS attributes
        /// @param nInterval the time interval between polls
        void RecheckMDSAttributes(int nInterval = 0);
        /// @brief A method for sending an MDS poll data request message
        void SendMDSPollDataRequest();
        
        /// @brief A method for repeatedly sending an MDSCreateEventResult so that the monitor keeps the connection alive
        /// @param nInterval the time interval between sends
        void KeepConnectionAlive(int nInterval = 0);
        
        /// @brief the Callback function to be called whenever a data packet has been received to process the data
        /// @param errorCode the error code from the receive operation
        /// @param numBytesReceived the number of bytes received
        /// @param overlapped the WSAOverlapped structure from the receive function
        /// @param flags the flags from the WSARecvFrom function
        void CALLBACK ReceiveCallback(DWORD errorCode, DWORD numBytesReceived, LPWSAOVERLAPPED overlapped, DWORD flags = 0);
        /// @brief A method to write the received data bytes to a file for logging
        /// @param path_to_file the path to the file
        /// @param bytes_string the bytes in string format
        /// @return true if no error occurred
        static bool ByteArrayToFile(const std::string& filename, const std::string& bytes_string);
        
        /// @brief A method to write the received data bytes to a file for logging
        /// @param path_to_file the path to the file
        /// @param data_bytes the bytes in form of a vector of std::bytes
        /// @param numBytesReceived the number of bytes received
        /// @return true if no errors occurred
        static bool ByteArrayToFile(const std::string& path_to_file, const std::vector<std::byte>& data_bytes, uint32_t numBytesReceived);

        bool stopThreadRecheckMDSAttributes = false;
        bool stopThreadKeepConnectionAlive = false;
        bool stopThreadReceive = false;
        int m_receiveInterval = RECEIVEINTERVAL;
        /// @brief the thread that repeatedly rechecks the MDS attributes
        /// @param nInterval the time interval
        void ThreadRecheckMDSAttributes(int nInterval = 0);
        /// @brief the thread that repeatedly makes sue the monitor keeps the connection alive
        /// @param nInterval the time interval
        void ThreadKeepConnectionAlive(int nInterval = 0);
        /// @brief the thread tha trepeatedly calls the Receive function to receive answers and wave and numeric data from the monitor
        /// @param receivedBuffer the buffer to pass to the Receive function
        void ThreadReceive(char* receivedBuffer);

        ///@brief Logging
        char* getLastMessage();

};

///current state of the client for the asynchronous receiving of data
class Receive_State 
{
    public:
        Receive_State() = default;
        ~Receive_State() = default;

        WSAOVERLAPPED overlapped; ///nötig um auf hEvent und overlap operation params zuzugreifen
        WSABUF wsaBuf;
        sockaddr_in state_ip;
        long unsigned int numBytesReceived;
        long unsigned int numBytesTransferred;
};
