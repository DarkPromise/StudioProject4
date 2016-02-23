#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "Component.h"

#define DRAG 1.5f

class PhysicsComponent : public Component
{
public:
	PhysicsComponent();
	virtual ~PhysicsComponent();

	void CreateComponent(luabridge::LuaRef & tableInfo, std::string name);
	void Update(double dt);

	void setAcceleration(float acc);
	float getAcceleration();

	void setForce(float force);
	float getForce();

	void setMass(float mass);
	float getMass();
private:
	float m_fForce;
	float m_fAcceleration;
	float m_fMass;
};

#endif