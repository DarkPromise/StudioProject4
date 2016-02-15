#ifndef NETWORKING_H
#define NETWORKING_H

#include <cstring> // memset
#include <string>
#include "Entity.h"

enum Packet_Types
{
	PACKET_MESSAGE = 0,
	PACKET_MOVEMENT,
	PACKET_REQUEST,
	PACKET_CONNECTED,
	PACKET_NEW,
	PACKET_UPDATE,
	PACKET_REJECTED,
};

typedef struct PacketInfo
{
	Packet_Types packetID;
	std::string m_sName;
	Vector3 m_v3Position;
	Vector3 m_v3Velocity;
	Vector3 m_v3Direction;
	Vector3 m_v3Rotation;
	float m_fSize;
	float m_fForce;
	float m_fAcceleration;
} packet;

#endif