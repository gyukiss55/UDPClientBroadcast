/*
C_UDPClient
*/

#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "C_UDPClient.h"


C_UDPClient::C_UDPClient()
{
    WORD wVersionRequested = MAKEWORD(1, 1);       // Stuff for WSA functions
    WSADATA wsaData;                              // Stuff for WSA functions
    // This stuff initializes winsock
    WSAStartup(wVersionRequested, &wsaData);

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
    int                  iOptVal;         // Socket option value
    int                  iOptLen;         // Socket option length
    iOptVal = 1;
    iOptLen = sizeof(int);
    setsockopt(client_s, SOL_SOCKET, SO_BROADCAST, (char*)&iOptVal, iOptLen);

}

C_UDPClient::~C_UDPClient()
{
    // Close all open sockets
    int retcode = closesocket(client_s);
    if (retcode < 0)
    {
        printf("*** ERROR - closesocket() failed \n");
        exit(-1);
    }

    // This stuff cleans-up winsock
    WSACleanup();
}

int C_UDPClient::SendToServer(const std::string& sendStr, std::string& reply)
{
    int                  addr_len;        // Internet address length
    char                 out_buf[4096];   // Output buffer for data
    char                 in_buf[4096];    // Input buffer for data
    int                  retcode;         // Return code
    // Assign a message to buffer out_buf
    strcpy_s(out_buf, sizeof(out_buf), sendStr.c_str ());

    // Now send the message to server.
    retcode = sendto(client_s, out_buf, (strlen(out_buf) + 1), 0,
        (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (retcode < 0)
    {
        printf("*** ERROR - sendto() failed \n");
        return (-1);
    }

    // Wait to receive a message
    addr_len = sizeof(server_addr);
    retcode = recvfrom(client_s, in_buf, sizeof(in_buf), 0,
        (struct sockaddr*)&server_addr, &addr_len);
    if (retcode < 0)
    {
        printf("*** ERROR - recvfrom() failed \n");
        return (-1);
    }

    // Output the received message
    printf("Received from server: %s \n", in_buf);
    reply = in_buf;
    return 0;

}
