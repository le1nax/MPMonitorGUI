#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include "Socket.h"

#define BUFFERSIZE 1024

//
struct UdpState {
    SocketClient state_client;
    sockaddr_in state_ip;
};


class SocketClient : public UDPSocket {

    public:
        SocketClient();
        ~SocketClient() = default;  

    private: 
        void establishLanConnection();
        void SendWaveAssociationRequest();
        void ProcessPacket(std::vector<std::byte> bytes);
        void sendBytes(std::vector<std::byte> bytes);

        std::vector<std::byte> m_readassocbuffer;
        std::vector<std::byte> m_readmdsconnectbuffer;
        std::string m_remoteIPtarget = "";
        unsigned short m_port = 0; 
        sockaddr_in m_sa_remoteIPtarget;
        char buffer[BUFFERSIZE];
        UdpState udpState; //current state of the client for the asynchronous receiving of data

        /// @todo make threads
        void SendCycledExtendedPollDataRequest();
        void SendCycledExtendedPollWaveDataRequest();
        void RecheckMDSAttributes();
        void KeepConnectionAlive();
        void BeginReceive(int flags = 0);
        static void CALLBACK ReceiveCallback(DWORD errorCode, DWORD numBytesReceived /*cbTransferred*/, LPWSAOVERLAPPED overlapped, DWORD flags)

};

#endif