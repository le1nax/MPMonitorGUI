// #ifndef SOCKET_CLIENT_H
// #define SOCKET_CLIENT_H

#include "Socket.h"


//
class SocketClient : public UDPSocket {

    public:
        SocketClient();
        ~SocketClient() = default;  

    private: 
        void establishLanConnection();
        void SendWaveAssociationRequest();
        void ProcessPacket(char* buffer);
        void sendBytes(std::vector<std::byte> bytes);

        std::vector<std::byte> m_readassocbuffer;
        std::vector<std::byte> m_readmdsconnectbuffer;
        std::string m_remoteIPtarget = "";
        std::string m_strTimestamp = "";
        std::string m_DeviceID= "";
        std::string m_idlabelstring = "";
        std::vector<NumericValResult> m_NumericValList;
        std::vector<std::string> m_NumValHeaders;
        uint16_t m_port = 0; 
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
        //sockaddr_in m_sa_remoteIPtarget

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
        void SendCycledExtendedPollDataRequest();
        void SendCycledExtendedPollWaveDataRequest();
        void RecheckMDSAttributes();
        void KeepConnectionAlive();
        void ExportDataToCSV();
        void SaveNumericValueList();
        void AddIDLabelToList()

};
