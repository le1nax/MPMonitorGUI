#pragma once

#include "SocketClient.h"

class Program{

    public:
        Program(SocketClient client);
        SocketClient m_client;

    private: 
        void establishLanConnection();

}