/*
C_UDPClient
*/
#pragma once

#ifndef _C_UDPCLIENT_H_
#define _C_UDPCLIENT_H_

#include <windows.h>
#include <string>

#define  PORT_NUM   1050    // Arbitrary port number for the server

class C_UDPClient {
	int                  client_s;        // Client socket descriptor
	struct sockaddr_in   server_addr;     // Server Internet address
public:
	C_UDPClient();
	~C_UDPClient();
	int SendToServer(const std::string& sendStr, std::string& reply);
};

#endif //_C_UDPCLIENT_H_