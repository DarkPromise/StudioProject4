#include "AIComponent.h"
#include "Entity.h"
#include "LuaReader.h"

AIComponent::AIComponent()
: m_eState(STATE_IDLE)
, m_eType(AI_UNDEFINED)
, m_eDifficulty(DIFFICULTY_NORMAL)
, m_iSightRadius(0.f)
, m_eTargetEntity(nullptr)
{

}

AIComponent::~AIComponent()
{

}

void AIComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{
	using namespace luabridge;

	auto aiState = tableInfo["aiState"];
	if (aiState.isString())
	{
		if (aiState.cast<std::string>() == "Idle")
		{
			this->setState(STATE_IDLE);
		}
		else if (aiState.cast<std::string>() == "Patrol")
		{
			this->setState(STATE_PATROL);
		}
		else if (aiState.cast<std::string>() == "Chase")
		{
			this->setState(STATE_CHASE);
		}
	}
	else
	{
		std::cout << "AIComponent.aiState for " + name + " is not a string!" << std::endl;
	}
	
	auto aiType = tableInfo["aiType"];
	if (aiType.isString())
	{
		if (aiType.cast<std::string>() == "Undefined")
		{
			this->setType(AI_UNDEFINED);
		}
		else if (aiType.cast<std::string>() == "Guard")
		{
			this->setType(AI_GUARD);
		}
	}
	else
	{
		std::cout << "AIComponent.aiType for " + name + " is not a string!" << std::endl;
	}

	auto aiDifficulty = tableInfo["aiDifficulty"];
	if (aiDifficulty.isString())
	{
		if (aiDifficulty.cast<std::string>() == "Easy")
		{
			this->setDifficulty(DIFFICULTY_EASY);
		}
		else if (aiDifficulty.cast<std::string>() == "Normal")
		{
			this->setDifficulty(DIFFICULTY_NORMAL);
		}
		else if (aiDifficulty.cast<std::string>() == "Hard")
		{
			this->setDifficulty(DIFFICULTY_HARD);
		}
	}
	else
	{
		std::cout << "AIComponent.aiDifficulty for " + name + " is not a string!" << std::endl;
	}

	auto aiSightRadius = tableInfo["aiSightRadius"];
	if (aiSightRadius.isNumber())
	{
		this->setSightRadius(aiSightRadius.cast<int>());
	}
	else
	{
		std::cout << "AIComponent.aiSightRadius for " + name + " is not a number!" << std::endl;
	}
}

void AIComponent::Update(double dt, GridMap * gridmap, Entity * thePlayer)
{
	if (this->getParent()->getComponent<WaypointComponent>())
	{
		auto playerInfo = thePlayer->getComponent<InformationComponent>();
		if (playerInfo)
		{
			float indexX = playerInfo->getPosition().x / (gridmap->getMapWidth() * gridmap->getTileSize()) * gridmap->getMapWidth();
			float indexY = playerInfo->getPosition().y / (gridmap->getMapHeight() * gridmap->getTileSize()) * gridmap->getMapHeight();
			int playerIndexX = (int)indexX;
			int playerIndexY = gridmap->getMapHeight() - (int)indexY;
		}
	}
	else
	{

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

void AIComponent::setDifficulty(AI_DIFFICULTY difficulty)
{
	this->m_eDifficulty = difficulty;
}

AIComponent::AI_DIFFICULTY AIComponent::getDifficulty()
{
	return this->m_eDifficulty;
}

void AIComponent::setSightRadius(int sightRadius)
{
	this->m_iSightRadius = sightRadius;
}

int AIComponent::getSightRadius()
{
	return this->m_iSightRadius;
}


void AIComponent::FindNearbyEntity(Entity * entity)
{
	
}

void AIComponent::MoveRandomly()
{
	
}
