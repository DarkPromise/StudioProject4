#ifndef CLIENT_H
#define CLIENT_H

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")

#include "Networking.h"

class Client
{
public:
	Client();
	Client(SOCKET s);
	~Client();

	void setSocket(SOCKET s);
	SOCKET getSocket();
	
	void setEntityID(int i) { this->m_iEntityID = i; }
	int getEntityID() { return this->m_iEntityID; }
private:
	SOCKET m_iSocket;
	int m_iEntityID;
};

#endif