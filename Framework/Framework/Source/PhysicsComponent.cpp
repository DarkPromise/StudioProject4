#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
: m_fForce(0.f)
, m_fAcceleration(0.f)
, m_fMass(1.f)
{

}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{

}

void PhysicsComponent::Update(double dt)
{
	//this->m_v3Direction.x = sin(Math::DegreeToRadian(-m_fRotation));
	//this->m_v3Direction.y = cos(Math::DegreeToRadian(-m_fRotation));
	//this->m_fAcceleration = this->m_fForce / this->m_fMass; // divided by mass
	//this->m_v3Velocity.x += this->m_v3Direction.x * this->m_fAcceleration * dt;
	//this->m_v3Velocity.y += this->m_v3Direction.y * this->m_fAcceleration * dt;
	//this->m_v3Position += this->m_v3Velocity * dt;

	//// Input Drag Codes
	//if ((this->m_v3Velocity.y) > 0.1)
	//{
	//	this->m_v3Velocity.y -= this->m_v3Velocity.y * (DRAG * dt); //Drag Positive y
	//	if (this->m_v3Velocity.y < 0.1)
	//	{
	//		this->m_v3Velocity.y = 0;
	//	}

	//}
	//if ((this->m_v3Velocity.y) < 0)
	//{
	//	this->m_v3Velocity.y -= this->m_v3Velocity.y * (DRAG * dt); //Drag Negative y
	//	if (this->m_v3Velocity.y > -0.1)
	//	{
	//		this->m_v3Velocity.y = 0;
	//	}
	//}
	//if ((this->m_v3Velocity.x) > 0.1)
	//{
	//	this->m_v3Velocity.x -= this->m_v3Velocity.x * (DRAG * dt); //Drag Positive x
	//	if (this->m_v3Velocity.x < 0.1)
	//	{
	//		this->m_v3Velocity.x = 0;
	//	}
	//}
	//if ((this->m_v3Velocity.x) < 0)
	//{
	//	this->m_v3Velocity.x -= this->m_v3Velocity.x * (DRAG * dt); //Drag Negative x
	//	if (this->m_v3Velocity.x > -0.1)
	//	{
	//		this->m_v3Velocity.x = 0;
	//	}
	//}

	//this->m_v3Velocity.x = Math::Clamp(this->m_v3Velocity.x, -500.f, 500.f);
	//this->m_v3Velocity.y = Math::Clamp(this->m_v3Velocity.y, -500.f, 500.f);
	//this->m_fForce = 0;

	//this->m_v3Position.x = Math::Wrap((float)this->m_v3Position.x, 0.f, (float)1080);
	//this->m_v3Position.y = Math::Wrap((float)this->m_v3Position.y, 0.f, (float)720);
}

void PhysicsComponent::setAcceleration(float acc)
{
	this->m_fAcceleration = acc;
}

float PhysicsComponent::getAcceleration()
{
	return this->m_fAcceleration;
}

void PhysicsComponent::setForce(float force)
{
	this->m_fForce = force;
}

float PhysicsComponent::getForce()
{
	return this->m_fForce;
}

void PhysicsComponent::setMass(float mass)
{
	this->m_fMass = mass;
}

float PhysicsComponent::getMass()
{
	return this->m_fMass;
}