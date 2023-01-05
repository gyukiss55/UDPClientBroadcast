//=========================================== file = udpClientBroadcast.c =====
//=  A message "client" program to demonstrate sockets programming            =
//=   - This is udpClient.c modified to use broadcast                         =
//=============================================================================
//=  Notes:                                                                   =
//=    1) This program conditionally compiles for Winsock and BSD sockets.    =
//=       Set the initial #define to WIN or BSD as appropriate.               =
//=    2) This program needs udpServer to be running on another host.         =
//=       Program udpServer must be started first.                            =
//=---------------------------------------------------------------------------=
//=  Example execution: (udpServer and udpClientBroadcast on host 127.0.0.1)  =
//=    Received from server: This is a reply message from SERVER to CLIENT    =
//=---------------------------------------------------------------------------=
//=  Build: Windows: Borland: bcc32 udpClientBroadcast.c                      =
//=                  Visual C cmd line: cl udpClientBroadcast.c wsock32.lib   =
//=                  MinGW: gcc udpClientBroadcast.c -lws2_32                 =
//=         Unix: gcc udpClientBroadcast.c -lnsl -o udpClientBroadcast        =
//=---------------------------------------------------------------------------=
//=  Execute: udpClientBroadcast                                              =
//=---------------------------------------------------------------------------=
//=  Author: Ken Christensen                                                  =
//=          University of South Florida                                      =
//=          WWW: http://www.csee.usf.edu/~christen                           =
//=          Email: christen@csee.usf.edu                                     =
//=---------------------------------------------------------------------------=
//=  History:  KJC (04/10/10) - Genesis (from udpClient.c)                    =
//=============================================================================
#define  WIN                // WIN for Winsock and BSD for BSD sockets

//----- Include files ---------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <string.h>         // Needed for memcpy() and strcpy()
#ifdef WIN
#include <windows.h>      // Needed for all Winsock stuff
#include <conio.h>      // _kbhit()

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#endif
#ifdef BSD
#include <sys/types.h>    // Needed for sockets stuff
#include <netinet/in.h>   // Needed for sockets stuff
#include <sys/socket.h>   // Needed for sockets stuff
#include <arpa/inet.h>    // Needed for sockets stuff
#include <fcntl.h>        // Needed for sockets stuff
#include <netdb.h>        // Needed for sockets stuff
#endif

#include "C_UDPClient.h"

//----- Defines ---------------------------------------------------------------
#define  PORT_NUM           1050  // Port number used

//===== Main program ==========================================================
void main1(void)
{
#ifdef WIN
    WORD wVersionRequested = MAKEWORD(1, 1);       // Stuff for WSA functions
    WSADATA wsaData;                              // Stuff for WSA functions
#endif
    int                  client_s;        // Client socket descriptor
    struct sockaddr_in   server_addr;     // Server Internet address
    int                  addr_len;        // Internet address length
    char                 out_buf[4096];   // Output buffer for data
    char                 in_buf[4096];    // Input buffer for data
    int                  retcode;         // Return code
    int                  iOptVal;         // Socket option value
    int                  iOptLen;         // Socket option length

#ifdef WIN
    // This stuff initializes winsock
    WSAStartup(wVersionRequested, &wsaData);
#endif

    // Create a socket
    client_s = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_s < 0)
    {
        printf("*** ERROR - socket() failed \n");
        exit(-1);
    }

    // Fill-in server socket's address information
    server_addr.sin_family = AF_INET;                 // Address family to use
    server_addr.sin_port = htons(PORT_NUM);           // Port num to use
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Need this for Broadcast

    // Set socket to use MAC-level broadcast
    iOptVal = 1;
    iOptLen = sizeof(int);
    setsockopt(client_s, SOL_SOCKET, SO_BROADCAST, (char*)&iOptVal, iOptLen);

    // Assign a message to buffer out_buf
    strcpy_s(out_buf, sizeof (out_buf),"Test message from CLIENT to SERVER");

    // Now send the message to server.
    retcode = sendto(client_s, out_buf, (strlen(out_buf) + 1), 0,
        (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (retcode < 0)
    {
        printf("*** ERROR - sendto() failed \n");
        exit(-1);
    }

    // Wait to receive a message
    addr_len = sizeof(server_addr);
    retcode = recvfrom(client_s, in_buf, sizeof(in_buf), 0,
        (struct sockaddr*)&server_addr, &addr_len);
    if (retcode < 0)
    {
        printf("*** ERROR - recvfrom() failed \n");
        exit(-1);
    }

    // Output the received message
    printf("Received from server: %s \n", in_buf);

    // Close all open sockets
#ifdef WIN
    retcode = closesocket(client_s);
    if (retcode < 0)
    {
        printf("*** ERROR - closesocket() failed \n");
        exit(-1);
    }
#endif
#ifdef BSD
    retcode = close(client_s);
    if (retcode < 0)
    {
        printf("*** ERROR - close() failed \n");
        exit(-1);
    }
#endif

#ifdef WIN
    // This stuff cleans-up winsock
    WSACleanup();
#endif
}

void main(void) {
    C_UDPClient client;
    for (int i = 0; i < 10000; i++) {
        SYSTEMTIME st;
        GetSystemTime(&st);
        std::string send = "Send to Server ";
        send += std::to_string(i + 1) + " ";
        send += std::to_string(st.wSecond) + " ";
        send += std::to_string(st.wMilliseconds) + " ";
        std::string rec;

        client.SendToServer(send, rec);

        OutputDebugStringA("Send:");
        OutputDebugStringA(send.c_str ());
        OutputDebugStringA("\n");
        OutputDebugStringA("Rec:");
        OutputDebugStringA(rec.c_str ());
        OutputDebugStringA("\n");
        GetSystemTime(&st);
        std::string timeStr;
        timeStr = std::string(" ") + std::to_string(st.wSecond) + std::string(" ") + std::to_string(st.wSecond);
        OutputDebugStringA(timeStr.c_str ());
        OutputDebugStringA("\n");
        Sleep(100);
        if (_kbhit()) {
            break;
        }
    }
}
