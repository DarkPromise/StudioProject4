#include "HealthComponent.h"
#include "LuaReader.h"

HealthComponent::HealthComponent()
: m_fHp(0.f)
, m_bHealthBar(false)
{

}

HealthComponent::~HealthComponent()
{

}

void HealthComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{
	auto minHp = tableInfo["minHp"];
	if (minHp.isNumber())
	{
		this->setHealth(minHp);
	}
	else
	{
		std::cout << "HealthComponent.minHp for " + name + " is not a number!" << std::endl;
	}

	auto maxHp = tableInfo["maxHp"];
	if (maxHp.isNumber())
	{
		this->setMaxHealth(maxHp);
	}
	else
	{
		std::cout << "HealthComponent.maxHp for " + name + " is not a number!" << std::endl;
	}
}

void HealthComponent::Update(double dt)
{

}

void HealthComponent::setHealth(float health)
{
	this->m_fHp = health;
}

float HealthComponent::getHealth()
{
	return this->m_fHp;
}

void HealthComponent::setMaxHealth(float maxHealth)
{
	this->m_fMaxHp = maxHealth;
}

float HealthComponent::getMaxHealth()
{
	return this->m_fMaxHp;
}

void HealthComponent::setHealthBar(bool status)
{
	this->m_bHealthBar = status;
}

bool HealthComponent::getHealthBar()
{
	return this->m_bHealthBar;
}


