#include "include/SocketClient.h"
#include "include/Program.h"

#include <string.h>
#include <thread>
#include <iostream>
#include <chrono>

#include <imgui.h>
#include <stdio.h>

#include <imgui.h>


#pragma comment (lib, "ws2_32.lib")

#define REMOTEIP "169.254.0.1"
#define REMOTEPORT 24105

#define LOCALIP "127.0.0.1"
#define LOCALPORT 69696

#define MAX_BUFFER_SIZE 2001

using namespace std;

// int main(int, char**)
// {
//     IMGUI_CHECKVERSION();
//     // ImGui::CreateContext();
//     // ImGuiIO& io = ImGui::GetIO();


//     // // Build atlas
//     // unsigned char* tex_pixels = nullptr;
//     // int tex_w, tex_h;
//     // io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

//     // for (int n = 0; n < 20; n++)
//     // {
//     //     printf("NewFrame() %d\n", n);
//     //     io.DisplaySize = ImVec2(1920, 1080);
//     //     io.DeltaTime = 1.0f / 60.0f;
//     //     ImGui::NewFrame();

//     //     static float f = 0.0f;
//     //     ImGui::Text("Hello, world!");
//     //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
//     //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//     //     ImGui::ShowDemoWindow(nullptr);

//     //     ImGui::Render();
//     // }

//     // printf("DestroyContext()\n");
//     // ImGui::DestroyContext();
//     return 0;
// }
uint16_t extractNumber(const char* charArray);
uint16_t convertBigEndianToLittleEndian(const char* charArray);
int main()
{
    try
   {
        unique_ptr<WSASession> Session = make_unique<WSASession>();

        const std::string s_remoteIP = REMOTEIP;
        const unsigned short remotePort = REMOTEPORT;
        unique_ptr<SocketClient> client = make_unique<SocketClient>(s_remoteIP, remotePort);

        client->establishLanConnection();
    }
    catch (std::exception &ex) //catch any occurring system errors
    {
        std::cout << ex.what();  //print error message
    }

    std::cin.get();

    // const char* buffer = "\x04\x01";
    // int ans = 0;
    // //ans = ReadByteValuesFromBuffer(buffer, 0, 3);
    // ans = Read16ByteValuesFromBuffer(buffer, 0);
    // std::cout << ans << std::endl;
    return 0;
}
// static uint16_t Read16ByteValuesFromBuffer(const char* buffer, size_t startIndex) {
//     uint16_t number = 0;
//     const char* buffer1 = buffer;

//     for (size_t i = startIndex; i < uInt16Size+startIndex; ++i) {
//         number = (number << 8) | static_cast<uint16_t>(buffer1[i]);
//     }
//     return number;
// }
uint16_t extractNumber(const char* charArray) {
    uint16_t number = (static_cast<uint8_t>(charArray[1]) << 8) | static_cast<uint8_t>(charArray[0]);
    return number;
}
uint16_t convertBigEndianToLittleEndian(const char* charArray) {
    uint16_t number = static_cast<uint16_t>(static_cast<uint8_t>(charArray[0])) << 8;
    number |= static_cast<uint8_t>(charArray[1]);
    return number;
}

// int main()
// {
//     try
//    {
//         unique_ptr<WSASession> Session = make_unique<WSASession>();

//         const std::string s_remoteIP = LOCALIP;
//         const unsigned short remotePort = LOCALPORT;
//         unique_ptr<SocketClient> client = make_unique<SocketClient>(s_remoteIP, remotePort);

//         client->SendWaveAssociationRequest();
//         std::cout << "sent" << std::endl;
//         // std::cin.get();// Wait for user input before exiting
//         char buffer[maxbuffersize];
//         std::cout << "begin receive" << std::endl;
//         client->Receive(buffer);
//     }
//     catch (std::exception &ex) //catch any occurring system errors
//     {
//         std::cout << ex.what();  //print error message
//     }
//     return 0;
// }


