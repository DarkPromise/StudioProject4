#include "EntityPlayer.h"

EntityPlayer::EntityPlayer()
: m_bOnGround(false)
{

}

EntityPlayer::~EntityPlayer()
{

}

void EntityPlayer::Update(double dt)
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

void EntityPlayer::Jump()
{
	auto infoC = this->getComponent<InformationComponent>();
	if (infoC)
	{
		if (m_bOnGround)
		{
			infoC->setUpwardsSpeed(JUMP_SPEED);
			m_bOnGround = false; // Better be safe than sorry
		}
	}
}

