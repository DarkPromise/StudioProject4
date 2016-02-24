#include "GameplayComponent.h"
#include "LuaReader.h"

GameplayComponent::GameplayComponent()
: m_bCurrLevel(1)
, m_bHasKey(false)
, m_bRestartLevel(false)
, m_levelCleared(false)
, m_showkeyRequired(false)
, m_showKeyRequiredTimer(0.0)
{

}

GameplayComponent::~GameplayComponent()
{

}

void GameplayComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{
	auto currLevel = tableInfo["currLevel"];
	if (currLevel.isNumber())
	{
		this->setCurrLevel(currLevel);
	}
	else
	{
		std::cout << "GameplayComponent.currLevel for " + name + " is not a number!" << std::endl;
	}

	auto hasKey = tableInfo["hasKey"];
	if (hasKey.isString())
	{
		if (hasKey.cast<std::string>() == "true")
		{
			this->setHasKey(true);
		}
		else
		{
			this->setHasKey(false);
		}
	}
	else
	{
		std::cout << "GameplayComponent.hasKey for " + name + " is not a string!" << std::endl;
	}
}

void GameplayComponent::Update(double dt)
{
	
}

void GameplayComponent::setCurrLevel(int level)
{
	this->m_bCurrLevel = level;
}

int GameplayComponent::getCurrLevel()
{
	return this->m_bCurrLevel;
}

void GameplayComponent::incrementLevel()
{
	this->m_bCurrLevel += 1;
}

void GameplayComponent::setHasKey(bool status)
{
	this->m_bHasKey = status;
}

bool GameplayComponent::getHasKey()
{
	return this->m_bHasKey;
}


void GameplayComponent::setLevelCleared(bool status)
{
	this->m_levelCleared = status;
}

bool GameplayComponent::getLevelCleared()
{
	return this->m_levelCleared;
}


void GameplayComponent::setKeyRequired(bool status)
{
	this->m_showkeyRequired = status;
}

bool GameplayComponent::getKeyRequired()
{
	return this->m_showkeyRequired;
}

void GameplayComponent::setKeyShowTimer(double time)
{
	this->m_showKeyRequiredTimer = time;
}

double GameplayComponent::getKeyShowTimer()
{
	return this->m_showKeyRequiredTimer;
}

void GameplayComponent::setRestartLevel(bool status)
{
	this->m_bRestartLevel = status;
}

bool GameplayComponent::getRestartLevel()
{
	return this->m_bRestartLevel;
}

void GameplayComponent::Reset()
{
	this->m_bHasKey = false;
	this->m_bRestartLevel = false;
	this->m_levelCleared = false;
	this->m_showkeyRequired = false;
	this->m_showKeyRequiredTimer = 0.0;
}