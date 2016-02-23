#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "Component.h"

class HealthComponent : public Component
{
public:
	HealthComponent();
	virtual ~HealthComponent();

	void CreateComponent(luabridge::LuaRef & tableInfo, std::string name);
	void Update(double dt);

	void setHealth(float health);
	float getHealth();

	void setMaxHealth(float maxHealth);
	float getMaxHealth();

	void setHealthBar(bool status);
	bool getHealthBar();
private:
	float m_fHp;
	float m_fMaxHp;
	bool m_bHealthBar;
};

#endif