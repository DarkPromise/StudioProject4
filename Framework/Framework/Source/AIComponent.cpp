#include "AIComponent.h"
#include "Entity.h"

AIComponent::AIComponent()
: m_eType(AI_NEUTRAL)
, m_eState(STATE_IDLE)
, m_MessageBoard(NULL)
, m_eTargetEntity(nullptr)
, m_dMovementDelay(0.0)
, m_dMovementDelay2(0.0)
, m_dAttackDelay(0.0)
// Testing AI
, randomAngle(0.f)
{

}

AIComponent::~AIComponent()
{

}

void AIComponent::Update(double dt, Entity * thePlayer)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();

	m_dMovementDelay += dt;
	m_dMovementDelay2 += dt;
	m_dAttackDelay += dt;

	// RNG for "Should I continue being Idle or should I move?"
	if (m_dMovementDelay2 > 1.0) // Move for 1s || Idle for 1s
	{
		if ((this->m_eState != STATE_SEARCH) && (this->m_eState != STATE_KILL))
		{
			int Move = Math::RandIntMinMax(1, 2);
			switch (Move)
			{
			case 1: // Move
				m_eState = STATE_MOVE;
				m_dMovementDelay2 = 0.0;
				break;
			case 2: // Dont Move
				this->m_eState = STATE_IDLE;
				infoC->setVelocity(Vector3(0.f, 0.f, 0.f));
				m_dMovementDelay2 = 0.0;
				break;
			}
		}
	}

	float distance;
	float angle;
	Vector3 rotation;

	switch (m_eState)
	{
	case STATE_IDLE:
		MoveRandomly();
		break;
	case STATE_MOVE:
		infoC->setVelocity(randomVelocity);
		break;
	case STATE_SEARCH:
		// Move Towards Player
		// Get Distance
		distance = abs((infoC->getPosition() - thePlayer->getComponent<InformationComponent>()->getPosition()).Length());
		if (distance >= 30.f)
		{
			angle = Math::RadianToDegree(atan2(thePlayer->getComponent<InformationComponent>()->getPosition().x - infoC->getPosition().x, thePlayer->getComponent<InformationComponent>()->getPosition().z - infoC->getPosition().z));
			rotation = infoC->getRotation();
			rotation.y = angle;
			infoC->setRotation(rotation);
			infoC->setVelocity(200.f);
		}
		else
		{
			infoC->setVelocity(0.f);
			this->m_eState = STATE_KILL;
		}
		break;
	case STATE_KILL:
		// Attack Player every ???seconds (atk speed?)
		distance = abs((infoC->getPosition() - thePlayer->getComponent<InformationComponent>()->getPosition()).Length());
		if (distance <= 30.f)
		{
			if (m_dAttackDelay > 0.5)
			{
				auto * healthComponent = thePlayer->getComponent<HealthComponent>();
				if (healthComponent)
				{
					healthComponent->setHealth(healthComponent->getHealth() - 1);
				}
				m_dAttackDelay = 0.0;
			}
		}
		else
		{
			this->m_eState = STATE_SEARCH;
		}
		break;
	}
}

void AIComponent::setType(AI_TYPE type)
{
	this->m_eType = type;
}

AIComponent::AI_TYPE AIComponent::getType()
{
	return this->m_eType;
}

void AIComponent::setState(AI_STATE state)
{
	this->m_eState = state;
}

AIComponent::AI_STATE AIComponent::getState()
{
	return this->m_eState;
}

void AIComponent::setMessageBoard(MessageBoard * messageBoard)
{
	this->m_MessageBoard = messageBoard;
}

MessageBoard * AIComponent::getMessageBoard()
{
	return this->m_MessageBoard;
}

void AIComponent::FindNearbyEntity(Entity * entity)
{
	if (!m_eTargetEntity)
	{
		if (entity->getComponent<InformationComponent>()->getType() == InformationComponent::ENTITY_TYPE::TYPE_PLAYER)
		{
			std::cout << abs((entity->getComponent<InformationComponent>()->getPosition()- this->getParent()->getComponent<InformationComponent>()->getPosition()).Length()) << std::endl;
		}
	}

}

void AIComponent::MoveRandomly()
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();

	if (randomAngle == 0)
	{
		if (m_dMovementDelay > 3.0)
		{
			// Generate a random direction
			if (infoC)
			{
				randomAngle = Math::RandFloatMinMax(infoC->getRotation().y, infoC->getRotation().y + 180.f);
				randomVelocity = Math::RandFloatMinMax(100.f, 120.f);
			}

			m_dMovementDelay = 0.0;
		}
	}
	else
	{
		if (infoC->getRotation().y < randomAngle)
		{
			infoC->applyRotation(8.f, 1); // 8.f is the rotation speed
			if (infoC->getRotation().y >= randomAngle)
			{
				randomAngle = 0.0;
			}
		}
		else if (infoC->getRotation().y > randomAngle)
		{
			infoC->applyRotation(-8.f, 1);
			if (infoC->getRotation().y <= randomAngle)
			{
				randomAngle = 0.0;
			}
		}
	}
}
