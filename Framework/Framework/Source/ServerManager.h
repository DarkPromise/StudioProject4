#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>

#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

#pragma comment(lib,"ws2_32.lib")

#include "Networking.h"
#include "Client.h"

#define WM_SOCKET (WM_USER + 1)
#define DEFAULT_PORT 12345
#define MAX_CLIENTS 2
#define BUFSIZE 1024

class StateMultiplayerGame;
class StateHost;

#include "EntityTest.h"

class ServerManager
{
public:
	enum NETWORK_TYPE
	{
		TYPE_CLIENT,
		TYPE_SERVER,
		TYPE_LISTEN_SERVER
	};

	ServerManager();
	ServerManager(StateHost * state);
	~ServerManager();

	// Server Functions
	bool Start(const char * ipAddress = "localhost", USHORT port = 12345, NETWORK_TYPE type = TYPE_SERVER);
	void CreateClient(const char * ipAddress, USHORT port);
	void CreateServer(const char * ipAddress, USHORT port);
	void CreateListenServer(const char * ipAddress, USHORT port);
	void Select();
	void Ping(PacketInfo packet, StateMultiplayerGame * state);
	void AddNewClient(SOCKET s);
	void CloseConnection(int index);

	// Event Select
	void compress_events(int Index);

	// Thread Functions
	void StartThreads(USHORT port);
	void CreateServerThread(USHORT port);
	void CreateClientThread(const char * ipAddress, USHORT port);

	// Get Set Functions
	void setNumberOfThreads(int i) { m_iNumberOfThreads = i; }
	int getNumberOfThreats() { return m_iNumberOfThreads; }

	char * getServerIP() { return inet_ntoa(this->m_iLocalIP); }
	char * getHostName() { return m_host->h_name; }
	USHORT getServerPort() { return ntohs(this->m_iServerAddress.sin_port); }
	
	void setCurrentWindow(HWND handle) { this->currWindow = handle; }

	bool getConnected() { return this->m_bConnected; }

	std::vector<Entity*> & getEntityList() { return this->m_entityList; }

	// Generic Functions
	char** ServerManager::splitText(char *string, char *delim, int *count);
private:
	// Client
	bool m_bConnected;
	fd_set ReadFds, TempFds;

	// State
	StateHost * m_parentState;

	// Window Info
	HWND currWindow;

	// Server Info
	WSAData wData;
	sockaddr_in m_iServerAddress; //Server Address
	HOSTENT * m_host;
	SOCKET m_iServerSocket;
	IN_ADDR m_iLocalIP;
	u_long m_iMode; //Connection Mode
	
	// Event Select
	WSAEVENT NewEvent;
	WSANETWORKEVENTS NetworkEvents;
	int Index;
	int ClientLen;
	SOCKET ClientSocket;
	SOCKADDR_IN ClientAddress;
	WSAEVENT g_EventArray[WSA_MAXIMUM_WAIT_EVENTS];
	char MessageBuffer[BUFSIZE];
	int MessageLen;

	// Server Data
	std::vector<Client*> m_clientList;
	std::vector<std::thread> m_threadList;
	std::vector<Entity*> m_entityList;
	int m_iNumberOfThreads;
};

#endif