#ifndef GAMEPLAY_COMPONENT_H
#define GAMEPLAY_COMPONENT_H

#include "Component.h"

class GameplayComponent : public Component
{
public:
	GameplayComponent();
	virtual ~GameplayComponent();

	void CreateComponent(luabridge::LuaRef & tableInfo);
	void Update(double dt);


public:
	void setCurrLevel(int level);
	int getCurrLevel();
	void incrementLevel();

	void setHasKey(bool status);
	bool getHasKey();

	void setLevelCleared(bool status);
	bool getLevelCleared();

	void setKeyRequired(bool status);
	bool getKeyRequired();

	void setKeyShowTimer(double time);
	double getKeyShowTimer();

private:
	int m_bCurrLevel;
	bool m_bHasKey;
	bool m_levelCleared;
	bool m_showkeyRequired;

	double m_showKeyRequiredTimer;
	double m_switchActivatedTimer;
};

#endif