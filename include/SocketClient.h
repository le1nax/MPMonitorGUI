#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include "Socket.h"

//
class SocketClient : public UDPSocket {

    public:
        SocketClient();
        ~SocketClient() = default;  

    private: 
        void establishLanConnection();
        void SendWaveAssociationRequest();
        void ProcessPacket(std::vector<std::byte>);

        std::vector<std::byte> readassocbuffer;
        std::vector<std::byte> readmdsconnectbuffer;

        /// @todo make threads
        void SendCycledExtendedPollDataRequest();
        void SendCycledExtendedPollWaveDataRequest();
        void RecheckMDSAttributes();
        void KeepConnectionAlive();

};
