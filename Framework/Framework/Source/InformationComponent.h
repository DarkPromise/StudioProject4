#ifndef INFORMATION_COMPONENT_H
#define INFORMATION_COMPONENT_H

#pragma warning(disable:4244)

#include "Component.h"

#include <string>
#include "Vector3.h"
#include "Branch.h"

#include "GridMap.h"

class InformationComponent : public Component
{
public:
	enum ENTITY_TYPE
	{
		TYPE_PLAYER,
		TYPE_NPC,
		TYPE_PROJ,
		TYPE_UNDEFINED,
		TYPE_TOTAL,
	};
	InformationComponent();
	virtual ~InformationComponent();

	void CreateComponent(luabridge::LuaRef & tableInfo, std::string name, GridMap * gridMap);
	void Update(double dt);

	void setName(const std::string name);
	std::string toString();

	int getUID();
	void setUID(int id) { this->m_iUID = id; }

	void setPosition(Vector3 position);
	Vector3 getPosition();

	void setVelocity(Vector3 velocity);
	Vector3 getVelocity();

	void setDirection(Vector3 direction);
	Vector3 getDirection();

	void setRotation(Vector3 rotate);
	Vector3 getRotation();
	void applyRotation(float rotateValue, int axis);

	void setType(ENTITY_TYPE type);
	void setType(std::string type);
	ENTITY_TYPE getType();

	void setUpwardsSpeed(float upwardsSpeed);
	float getUpwardsSpeed();

	// Because I am lazy (to create a new component just for AI Assignment)
	void setAmmoCount(int i) { this->m_iAmmoCount = i; }
	void setMaxAmmo(int i) { this->m_iMaxAmmo = i; }
	int getAmmo() { return this->m_iAmmoCount; }
	int getMaxAmmo() { return this->m_iMaxAmmo; }

	Branch * partition;
private:
	std::string m_sName;
	int m_iUID;
	Vector3 m_v3Position;
	Vector3 m_v3Velocity;
	Vector3 m_v3Direction;
	Vector3 m_v3Rotation;
	float m_fUpwardsSpeed;
	ENTITY_TYPE m_type;

	// Lazy Members
	int m_iAmmoCount;
	int m_iMaxAmmo;
};

#endif