#include "InformationComponent.h"
#include "EntityPlayer.h"

InformationComponent::InformationComponent()
: m_sName("NULL")
, m_iAmmoCount(30.f)
, m_iMaxAmmo(30.f)
, m_fUpwardsSpeed(0.f)
, m_v3Rotation(Vector3(0.f,0.f,0.f))
, m_v3Position(Vector3(0.f, 0.f, 0.f))
, m_v3Velocity(Vector3(0.f, 0.f, 0.f))
, m_v3Direction(Vector3(0.f, 0.f, 0.f))
, m_type(ENTITY_TYPE::TYPE_UNDEFINED)
{
	// Unique ID Generation
	this->m_iUID = size_t(this);
}

void InformationComponent::CreateComponent(luabridge::LuaRef& tableInfo)
{
	using namespace luabridge;
	auto infoName = tableInfo["name"];
	if (infoName.isString())
	{
		this->setName(infoName.cast<std::string>());
	}
	else
	{
		std::cout << "InformationComponent.name is not a string!" << std::endl;
	}

	auto infoPosition = tableInfo["position"];
	if (infoPosition.isTable())
	{
		if (infoPosition.length() == 3)
		{
			// LuaRef starts indexes from 1, not 0
			this->setPosition(Vector3(infoPosition.rawget<float>(1), infoPosition.rawget<float>(2), infoPosition.rawget<float>(3)));
		}
		else
		{
			std::cout << "InformationComponent.position is not an array with 3 values!" << std::endl;
		}
	}

	auto infoVelocity = tableInfo["velocity"];
	if (infoVelocity.isTable())
	{
		if (infoVelocity.length() == 3)
		{
			// LuaRef starts indexes from 1, not 0
			this->setVelocity(Vector3(infoVelocity.rawget<float>(1), infoVelocity.rawget<float>(2), infoVelocity.rawget<float>(3)));
		}
		else
		{
			std::cout << "InformationComponent.velocity is not an array with 3 values!" << std::endl;
		}
	}

	auto infoDirection = tableInfo["direction"];
	if (infoDirection.isTable())
	{
		if (infoDirection.length() == 3)
		{
			// LuaRef starts indexes from 1, not 0
			this->setDirection(Vector3(infoDirection.rawget<float>(1), infoDirection.rawget<float>(2), infoDirection.rawget<float>(3)));
		}
		else
		{
			std::cout << "InformationComponent.direction is not an array with 3 values!" << std::endl;
		}
	}

	auto infoRotation = tableInfo["rotation"];
	if (infoRotation.isTable())
	{
		if (infoRotation.length() == 3)
		{
			// LuaRef starts indexes from 1, not 0
			this->setRotation(Vector3(infoRotation.rawget<float>(1), infoRotation.rawget<float>(2), infoRotation.rawget<float>(3)));
		}
		else
		{
			std::cout << "InformationComponent.rotation is not an array with 3 values!" << std::endl;
		}
	}

	auto infoType = tableInfo["type"];
	if (infoType.isString())
	{
		this->setType(infoType.cast<std::string>());
	}
	else
	{
		std::cout << "InformationComponent.type is not a string!" << std::endl;
	}
}

InformationComponent::~InformationComponent()
{
}

void InformationComponent::Update(double dt)
{
	this->getParent()->Update(dt);

	if (this->m_type == TYPE_PLAYER || this->m_type == TYPE_NPC)
	{
		
	}
	else
	{
		this->m_v3Position += this->m_v3Velocity * dt;
	}

	if (partition != NULL)
	{
		// If the partition has other parts (Nodes)
		// Or if the Entity cant fit into the domain
		if ((partition->hasChild()) || (partition->canContain(this->getParent()) == false))
		{
			partition->removeObject(this->getUID());
			// Update the tree
			partition->getRoot()->addObject(this->getParent());
		}
	}
}

void InformationComponent::setName(const std::string name)
{
	this->m_sName = name;
}

std::string InformationComponent::toString()
{
	return m_sName;
}

int InformationComponent::getUID()
{
	return this->m_iUID;
}

void InformationComponent::setPosition(Vector3 position)
{
	this->m_v3Position = position;
}

Vector3 InformationComponent::getPosition()
{
	return this->m_v3Position;
}

void InformationComponent::setVelocity(Vector3 velocity)
{
	this->m_v3Velocity = velocity;
}

Vector3 InformationComponent::getVelocity()
{
	return this->m_v3Velocity;
}

void InformationComponent::setDirection(Vector3 direction)
{
	this->m_v3Direction = direction;
}

Vector3 InformationComponent::getDirection()
{
	return this->m_v3Direction;
}

void InformationComponent::setRotation(Vector3 rotate)
{
	this->m_v3Rotation = rotate;
}

Vector3 InformationComponent::getRotation()
{
	return this->m_v3Rotation;
}

void InformationComponent::applyRotation(float rotateValue, int axis)
{
	switch (axis)
	{
	case 0:
		// apply to X-axis
		this->m_v3Rotation.x += (rotateValue);
		break;
	case 1:
		// apply to Y-Axis
		this->m_v3Rotation.y += (rotateValue);
		break;
	case 2:
		// apply to X-Axis
		this->m_v3Rotation.z += (rotateValue);
		break;
	}
}

void InformationComponent::setType(ENTITY_TYPE type)
{
	this->m_type = type;
}

void InformationComponent::setType(std::string type)
{
	if (type == "Player")
	{
		this->m_type = TYPE_PLAYER;
	}
	else if (type == "NPC")
	{
		this->m_type = TYPE_NPC;
	}
	else
	{
		this->m_type = TYPE_UNDEFINED;
	}
}

InformationComponent::ENTITY_TYPE InformationComponent::getType()
{
	return this->m_type;
}

void InformationComponent::setUpwardsSpeed(float upwardsSpeed)
{
	this->m_fUpwardsSpeed = upwardsSpeed;
}

float InformationComponent::getUpwardsSpeed()
{
	return this->m_fUpwardsSpeed;
}