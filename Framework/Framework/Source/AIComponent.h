#ifndef AI_COMPONENT_H
#define AI_COMPONENT_H

#include "Component.h"
#include "GridMap.h"
#include "MessageBoard.h"

#define LOOK_DELAY 0.1
#define MOVE_DELAY 0.25
#define SIGHT_DELAY 0.3
#define SWITCH_DELAY 0.5

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
		STATE_PATHING,
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

	void setSightLength(int SightLength);
	int getSightLength();

	void setMessageBoard(MessageBoard * messageBoard);
	MessageBoard * getMessageBoard();

	void FindNearbyEntity(GridMap * gridMap, Entity * entity);
	void MoveRandomly();
	void PatrolAround(GridMap * gridMap, double dt);
	void LookAround(double dt);
	void LookLeft();
	void LookRight();
	void RenderLineOfSight(View * theView);
	void ChaseEntity(GridMap * gridMap, Entity * entity, double dt);
	void WalkToPoint(GridMap * gridMap, Entity * entity, double dt);
private:
	AI_TYPE m_eType;
	AI_STATE m_eState;
	AI_DIFFICULTY m_eDifficulty;
	Entity * m_eTargetEntity;
	int m_iSightLength;
	int m_iMoveCount;
	Mesh * m_cSightMesh;

	double m_dLookDelay;
	double m_dMoveDelay;
	double m_dSightDelay;

	bool m_bPathing;

	std::vector<Vector3> m_PathList;
};

#endif