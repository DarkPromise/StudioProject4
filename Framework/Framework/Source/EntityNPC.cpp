#include "EntityNPC.h"

EntityNPC::EntityNPC()
: m_bOnGround(false)
{

}

EntityNPC::~EntityNPC()
{

}

void EntityNPC::Test()
{
	std::cout << "Lua Test" << std::endl;
}

void EntityNPC::Update(double dt)
{
	auto infoC = this->getComponent<InformationComponent>();
	auto collisionC = this->getComponent<CollisionComponent>();
	if (infoC)
	{
		if (collisionC)
		{
			if (infoC->getPosition().y <= collisionC->getAABB()->Max.y + 5.f)
			{
				m_bOnGround = true;
			}
			else
			{
				m_bOnGround = false;
			}
		}
		else
		{
			if (infoC->getPosition().y <= collisionC->getAABB()->Max.y * 0.5f)
			{
				m_bOnGround = true;
			}
			else
			{
				m_bOnGround = false;
			}
		}
	}
}

void EntityNPC::Jump()
{
	auto infoC = this->getComponent<InformationComponent>();
	if (infoC)
	{
		if (m_bOnGround)
		{
			infoC->setUpwardsSpeed(JUMP_SPEED);
			m_bOnGround = false;
		}
	}
}

