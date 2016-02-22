#include "ServerManager.h"

ServerManager::ServerManager()
: m_iNumberOfThreads(0) // Default 0 Threads
, Index(0)
, ClientLen(0)
, m_bConnected(false)
{

}

ServerManager::ServerManager(StateHost * state)
: m_iNumberOfThreads(0) // Default 0 Threads
, Index(0)
, ClientLen(0)
{
	this->m_parentState = state;
}

ServerManager::~ServerManager()
{
	if (!m_entityList.empty())
	{
		for (int i = 0; i < m_entityList.size(); i++)
		{
			delete m_entityList[i];
		}
	}
	m_entityList.clear();
	m_entityList.~vector();

	if (!m_clientList.empty())
	{
		for (int i = 0; i < m_clientList.size(); i++)
		{
			delete m_clientList[i];
		}
	}
	m_clientList.clear();
	m_clientList.~vector();

	WSACleanup();
}

bool ServerManager::Start(const char * ipAddress, USHORT port, NETWORK_TYPE type)
{
	int err = WSAStartup(0x0202, &wData);
	if (err)
	{
		std::cout << "WSAStartup Failed" << std::endl;
		return false;
	}
	if (wData.wVersion != 0x0202)
	{
		std::cout << "Outdated Winsock Version" << std::endl;
		WSACleanup();
		return false;
	}

	m_host = gethostbyname("");
	if (m_host == NULL)
	{
		std::cout << "Failed to get Host Name" << std::endl;
		return false;
	}

	switch (type)
	{
	case NETWORK_TYPE::TYPE_CLIENT:
		//Create Client Thread
		if (ipAddress == "localhost")
		{
			CreateClient("127.0.0.1", port);
		}
		else
		{
			CreateClient(ipAddress, port);
		}
		break;
	case NETWORK_TYPE::TYPE_SERVER:
		//Create Multithreaded Server
		CreateServer(ipAddress, port);
		break;
	case NETWORK_TYPE::TYPE_LISTEN_SERVER:
		//Create ListenServer
		CreateListenServer(ipAddress, port);
		break;
	}

	return true;
}

void ServerManager::CreateClient(const char * ipAddress, USHORT port)
{
	// Create Client Thread
	m_threadList.push_back(std::thread(&ServerManager::CreateClientThread, this, "127.0.0.1", port));
	m_threadList[0].join();
}

void ServerManager::CreateServer(const char * ipAddress, USHORT port)
{
	// Create Server Thread
	m_threadList.push_back(std::thread(&ServerManager::CreateServerThread, this, port));
	m_threadList[0].join();
}

void ServerManager::CreateListenServer(const char * ipAddress, USHORT port)
{
	// Create Server Thread First
	m_threadList.push_back(std::thread(&ServerManager::CreateServerThread,this,port));
	m_threadList[0].join();
}


void ServerManager::StartThreads(USHORT port)
{
	
}

void ServerManager::CreateServerThread(USHORT port)
{
	// Decided to go with TCP since its just two clients

	m_iServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	m_iServerAddress.sin_family = AF_INET;
	m_iServerAddress.sin_port = htons(port);
	memcpy(&(m_iLocalIP), m_host->h_addr, m_host->h_length);
	m_iServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_iServerSocket, (SOCKADDR*)&m_iServerAddress, sizeof(m_iServerAddress)) == SOCKET_ERROR)
	{
		std::cout << "Bind Failed" << std::endl;
		return;
	}

	NewEvent = WSACreateEvent();
	if (WSAEventSelect(m_iServerSocket, NewEvent, FD_ACCEPT) == SOCKET_ERROR) 
	{
		std::cout << "WSAEventSelect Failed with FD_ACCEPT" << std::endl;
	}

	if (listen(m_iServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Listen Failed" << std::endl;
		return;
	}

	Client * newClient = new Client();
	newClient->setSocket(m_iServerSocket);
	g_EventArray[m_clientList.size()] = NewEvent;
	m_clientList.push_back(newClient);

	std::cout << "Server Started" << std::endl;
}

void ServerManager::CreateClientThread(const char * ipAddress, USHORT port)
{
	// Decided to go with TCP since its just two clients

	m_iServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	m_iServerAddress.sin_family = AF_INET;
	m_iServerAddress.sin_port = htons(port);
	m_iServerAddress.sin_addr.s_addr = inet_addr(ipAddress);

	if (connect(m_iServerSocket, (SOCKADDR*)&m_iServerAddress, sizeof(m_iServerAddress)) == SOCKET_ERROR)
	{
		closesocket(m_iServerSocket);
		std::cout << "Failed to Connect to " << ipAddress << std::endl;
		WSACleanup();
		return;
	}

	FD_ZERO(&ReadFds);
	FD_SET(m_iServerSocket, &ReadFds);

	m_bConnected = true;
	std::cout << "Successfully Connected" << std::endl;
}

void ServerManager::Ping(PacketInfo packet, StateMultiplayerGame * state)
{
	// Create Data from Packet
	char * dataTo = new char[sizeof(PacketInfo)];
	memcpy(dataTo, &packet, sizeof(PacketInfo));
	// Send
	send(m_iServerSocket, dataTo, sizeof(PacketInfo), 0);
	
	/*------------------------------------------------------*/
	
	TempFds = ReadFds;
	TIMEVAL Timeout;
	Timeout.tv_sec = 0;
	Timeout.tv_usec = 1000;
	int Return;
	char * dataFrom = new char[sizeof(PacketInfo)];

	if (SOCKET_ERROR == (Return = select(0, &TempFds, 0, 0, &Timeout)))
	{
		closesocket(m_iServerSocket);
	}
	else if (0 > Return)
	{
		std::cout << "Select returned Error!" << std::endl;
	}
	else if (0 < Return)
	{
		Return = recv(m_iServerSocket, dataFrom, sizeof(PacketInfo), 0);
		if (0 > Return)
		{
			closesocket(m_iServerSocket);
			std::cout << "Exceptional Error" << std::endl;
			return;
		}
		else if (0 == Return)
		{
			closesocket(m_iServerSocket);
			std::cout << "Recieved Connection Closed" << std::endl;
			return;
		}
		else
		{
			PacketInfo newPacket;
			memcpy(&newPacket, dataFrom, sizeof(PacketInfo));
			if (newPacket.packetID == PACKET_REJECTED)
			{
				m_bConnected = false;
			}
			else if (newPacket.packetID == PACKET_NEW)
			{
				//state->createNewEntity();
			}
			else if (newPacket.packetID == PACKET_UPDATE)
			{
				//auto infoC = state->getOtherPlayer()->getComponent<InformationComponent>();
				////infoC->setAcceleration(newPacket.m_fAcceleration);
				//infoC->setDirection(newPacket.m_v3Direction);
				////infoC->setForce(newPacket.m_fForce);
				//infoC->setName(newPacket.m_sName);
				//infoC->setPosition(newPacket.m_v3Position);
				//infoC->setRotation(newPacket.m_v3Rotation);
				//infoC->setSize(newPacket.m_fSize);
			}
		}
	}
}

void ServerManager::Select()
{
	Index = WSAWaitForMultipleEvents(m_clientList.size(), g_EventArray, FALSE, 0, TRUE);
	Index = Index - WSA_WAIT_EVENT_0;

	for (int i = Index; i < m_clientList.size(); i++)
	{ 
		Index = WSAWaitForMultipleEvents(1, &g_EventArray[i], TRUE, 0, FALSE);

		if ((WSA_WAIT_FAILED == Index) || (WSA_WAIT_TIMEOUT == Index))
		{
			continue;
		}
		else
		{
			Index = i;
			WSAEnumNetworkEvents(m_clientList[Index]->getSocket(), g_EventArray[Index], &NetworkEvents);
			
			// New Connection
			if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
			{
				if (0 != NetworkEvents.iErrorCode[FD_ACCEPT_BIT])
				{
					std::cout << "FD_ACCEPT ERROR" << std::endl;
					break;
				}
				ClientLen = sizeof(ClientAddress);
				ClientSocket = accept(m_clientList[Index]->getSocket(), (SOCKADDR *)&ClientAddress, &ClientLen);
				if (INVALID_SOCKET == ClientSocket)
				{
					std::cout << "accept() Failed" << std::endl;
					continue;
				}
				if (m_clientList.size() < 3)
				{
					AddNewClient(ClientSocket);
				}
				else
				{
					PacketInfo newPacket;
					newPacket.packetID = PACKET_REJECTED;
					char * dataTo = new char[sizeof(PacketInfo)];
					memcpy(dataTo, &newPacket, sizeof(PacketInfo));
					send(ClientSocket, dataTo, sizeof(PacketInfo), 0);
				}
			}

			// Message Recieved
			if (NetworkEvents.lNetworkEvents & FD_READ)
			{
				if (0 != NetworkEvents.iErrorCode[FD_READ_BIT])
				{
					std::cout << "FD_READ ERROR" << std::endl;
					continue;
				}

				MessageLen = recv(m_clientList[Index - WSA_WAIT_EVENT_0]->getSocket(), MessageBuffer, BUFSIZE, 0);
				if (0 > MessageLen)
				{
					CloseConnection(Index);
					continue;
				}
				// Packet Recieved
				// Read Packet Data and update accordingly
				PacketInfo newPacket;
				memcpy(&newPacket, MessageBuffer, sizeof(PacketInfo));
				if (newPacket.packetID == PACKET_MOVEMENT)
				{
					// Update Entity
					auto infoC = m_entityList[m_clientList[Index]->getEntityID() - 1]->getComponent<InformationComponent>();
					//infoC->setAcceleration(newPacket.m_fAcceleration);
					infoC->setDirection(newPacket.m_v3Direction);
					//infoC->setForce(newPacket.m_fForce);
					infoC->setName(newPacket.m_sName);
					infoC->setPosition(newPacket.m_v3Position);
					infoC->setRotation(newPacket.m_v3Rotation);
				}
			}

			// Send to Clients Updates about the other clients

			for (unsigned int j = 1; j < m_clientList.size(); j++)
			{
				if (m_clientList[i]->getSocket() != m_clientList[j]->getSocket())
				{
					PacketInfo newPacket;
					newPacket.packetID = PACKET_UPDATE;
					char * dataTo = new char[sizeof(PacketInfo)];

					auto infoC = m_entityList[m_clientList[j]->getEntityID() - 1]->getComponent<InformationComponent>();
					newPacket.m_sName = infoC->toString();
					newPacket.m_v3Position = infoC->getPosition();
					newPacket.m_v3Velocity = infoC->getVelocity();
					newPacket.m_v3Direction = infoC->getDirection();
					newPacket.m_v3Rotation = infoC->getRotation();
					//newPacket.m_fForce = infoC->getForce();
					//newPacket.m_fAcceleration = infoC->getAcceleration();
					memcpy(dataTo, &newPacket, sizeof(PacketInfo));
					// Send
					send(m_clientList[i]->getSocket(), dataTo, sizeof(PacketInfo), 0);
					break;
				}
			}

			// Connection Closed
			if (NetworkEvents.lNetworkEvents & FD_CLOSE)
			{
				if (0 != NetworkEvents.iErrorCode[FD_CLOSE_BIT])
				{
					CloseConnection(Index);
					continue;
				}
				CloseConnection(Index);
			}
		}
	}
}

void ServerManager::AddNewClient(SOCKET s)
{
	Client * newClient = new Client();
	newClient->setSocket(s);
	newClient->setEntityID(m_clientList.size());
	WSAEVENT NewEvent;
	NewEvent = WSACreateEvent();
	WSAEventSelect(ClientSocket, NewEvent, FD_READ | FD_CLOSE);
	g_EventArray[m_clientList.size()] = NewEvent;
	m_clientList.push_back(newClient);
	//this->m_parentState->createNewEntity();

	for (int i = 1; i < m_clientList.size(); i++)
	{
		for (int j = 1; j < m_clientList.size(); j++)
		{
			if (m_clientList[i]->getSocket() == m_clientList[j]->getSocket())
			{
				continue;
			}
			PacketInfo newPacket;
			newPacket.packetID = PACKET_NEW;
			char * dataTo = new char[sizeof(PacketInfo)];
			memcpy(dataTo, &newPacket, sizeof(PacketInfo));
			// Send
			send(m_clientList[i]->getSocket(), dataTo, sizeof(PacketInfo), 0);
		}
	}

	std::cout << "New Client Added" << std::endl;
}

void ServerManager::CloseConnection(int Index)
{
	WSACloseEvent(g_EventArray[Index]);
	closesocket(m_clientList[Index]->getSocket());
	compress_events(Index);
	m_clientList.erase(m_clientList.begin() + Index);
	std::cout << "Connection Closed at Index : " << Index << std::endl;
	delete m_entityList[Index - 1];
	m_entityList.erase(m_entityList.begin() + Index - 1);
}

void ServerManager::compress_events(int OmitIndex)
{
	int i;

	for (i = OmitIndex; i < m_clientList.size(); i++)
	{
		g_EventArray[i] = g_EventArray[i + 1];
	}
	g_EventArray[i] = 0;
}

char** ServerManager::splitText(char *string, char *delim, int *count)
{
	char **retval = NULL;
	char *token = NULL;
	char *context = NULL;

	*count = 0;
	token = strtok_s(string, delim, &context);

	while (token != NULL)
	{
		*count = *count + 1;
		retval = (char**)realloc(retval, (*count * sizeof(char*)));
		retval[*count - 1] = _strdup(token);
		token = strtok_s(NULL, delim, &context);
	}

	return retval;
}