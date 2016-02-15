#include "Client.h"

Client::Client()
{

}

Client::~Client()
{

}

void Client::setSocket(SOCKET s)
{
	this->m_iSocket = s;
}

SOCKET Client::getSocket()
{
	return this->m_iSocket;
}