#ifndef AI_COMPONENT_H
#define AI_COMPONENT_H

#include "Component.h"
#include "GridMap.h"
#include "MessageBoard.h"

class AIComponent : public Component
{
public:
	enum AI_TYPE
	{
		AI_GUARD,
		AI_UNDEFINED,
	};

	enum AI_DIFFICULTY
	{
		DIFFICULTY_EASY,
		DIFFICULTY_NORMAL,
		DIFFICULTY_HARD,
	};

	enum AI_STATE
	{
		STATE_IDLE,
		STATE_PATROL,
		STATE_CHASE,
	};

	AIComponent();
	virtual ~AIComponent();

	void CreateComponent(luabridge::LuaRef & tableInfo, std::string name);
	void Update(double dt, GridMap * gridmap, Entity * thePlayer);

	void setType(AI_TYPE type);
	AI_TYPE getType();

	void setState(AI_STATE state);
	AI_STATE getState();

	void setDifficulty(AI_DIFFICULTY difficulty);
	AI_DIFFICULTY getDifficulty();

	void setSightRadius(int sightRadius);
	int getSightRadius();

	void setMessageBoard(MessageBoard * messageBoard);
	MessageBoard * getMessageBoard();

	void FindNearbyEntity(Entity * entity);
	void MoveRandomly();
private:
	AI_TYPE m_eType;
	AI_STATE m_eState;
	AI_DIFFICULTY m_eDifficulty;
	Entity * m_eTargetEntity;
	int m_iSightRadius;
};

#endif