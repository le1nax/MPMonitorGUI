#pragma once

#include "Socket.h"


class SocketClient : public UDPSocket {

    public:
        SocketClient(std::string remoteIPtarget, const unsigned short remotePortTarget);
        ~SocketClient() = default;
        
        void establishLanConnection();

    private: 
        /*class UdpState{
            public: 
                UdpState(SocketClient& parent) : state_client(parent) {}
            private:
                friend class SocketClient;
                SocketClient& state_client;
                sockaddr_in state_ip;
        } m_udpState; //current state of the client for the asynchronous receiving of data*/
        void SendWaveAssociationRequest();
        void ProcessPacket(char* buffer);
        void sendBytes(std::vector<std::byte> bytes);
        std::vector<std::byte> m_readassocbuffer;
        std::vector<std::byte> m_readmdsconnectbuffer;
        std::string m_remoteIPtarget = "";
        sockaddr_in m_sa_remoteIPtarget;
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
        tm m_baseDateTime{};
        tm GetAbsoluteTimeFromBCDFormat(char* bcdtimebuffer);

        void initMsgs();
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

        /// @todo make threads
        void CheckLinkedPollPacketActionType(char* buffer);

        int DecodePollObjects(PollInfoList* pollobjects, char* packetbuffer);
        int DecodeSingleContextPollObjects(SingleContextPoll* scpoll, char* buffer);
        int DecodeObservationPollObjects(ObservationPoll* scpoll, char* buffer);

        void ReadIDLabel(char* buffer);
        void ReadNumericObservationValue(char* buffer);
        void ReadCompoundNumericObsValue(char* buffer);
        void ReadIDLabelString(char* buffer);
        void ReadWaveSaObservationValueObject(char* buffer);
        void ReadCompoundWaveSaObservationValue(char* buffer);
        void ReadSaSpecifications(char* buffer);
        void ReadSaScaleSpecifications(char* buffer);
        void ReadSaCalibrationSpecifications(char* buffer);
        void ReadWaveSaObservationValue(char* buffer);
        
        void DecodeAvaObjects(std::unique_ptr<AvaObj> avaObj, char* buffer);
        void PollPacketDecoder(char* packetbuffer, size_t headersize);
        void ParseMDSCreateEventReport(char* buffer);
        void GetBaselineRelativeTimestamp(char* timebuffer);
        void DecodeMDSAttribObjects(std::unique_ptr<AvaObj> avaobj,  char* attriblistobjects_buffer, size_t count);
        void SendMDSCreateEventResult();
        void CheckPollPacketActionType(char* buffer);
        void SendRTSAPriorityMessage(std::vector<std::byte> WaveTrType);
        void SendCycledExtendedPollDataRequest(size_t nInterval);
        void SendCycledExtendedPollWaveDataRequest(size_t nInterval);
        void ExportDataToCSV();
        void CreateWaveformSet(size_t nWaveSetType, std::vector<std::byte> WaveTrtype);
        void GetRTSAPriorityListRequest();
        void SetRTSAPriorityList(size_t nWaveSetType);
        void SaveNumericValueList();
        void AddIDLabelToList();
        void RecheckMDSAttributes(int nInterval = 0);
        void SendMDSPollDataRequest();
        void KeepConnectionAlive(int nInterval = 0);
        void Receive(char* buffer, size_t len, int flags = 0);
        void CALLBACK ReceiveCallback(DWORD errorCode, DWORD numBytesReceived, LPWSAOVERLAPPED overlapped, DWORD flags = 0);
        static bool ByteArrayToFile(const std::string& filename, const std::string& bytes_string);
        static bool ByteArrayToFile(const std::string& path_to_file, const std::vector<std::byte>& data_bytes, uint32_t numBytesReceived);
};

//current state of the client for the asynchronous receiving of data
class Receive_State 
{
    public:
        Receive_State() = default;
        ~Receive_State() = default;

        WSAOVERLAPPED overlapped; //noetig um auf hEvent und overlap operation params zuzugreifen
        char* buffer;
        sockaddr_in state_ip;
        long unsigned int numBytesReceived;
        long unsigned int numBytesTransferred;
};
