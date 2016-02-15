#ifndef AI_COMPONENT_H
#define AI_COMPONENT_H

#include "Component.h"
#include "MessageBoard.h"

class AIComponent : public Component
{
public:
	enum AI_TYPE
	{
		AI_SUPPORT, // Medic and Ammo
		AI_SOLDIER,
		AI_GUNNER,
		AI_ENEMY,
		AI_WALL,
		AI_NEUTRAL,
	};

	enum AI_STATE
	{
		STATE_IDLE,
		STATE_MOVE,
		STATE_SEARCH,
		STATE_KILL,
		STATE_DEFEND,
		STATE_SUPPORTING,
		STATE_REQUEST_HELP,
		STATE_REQUEST_SUPPLIES,
	};

	AIComponent();
	virtual ~AIComponent();

	void Update(double dt, Entity * thePlayer);

	void setType(AI_TYPE type);
	AI_TYPE getType();

	void setState(AI_STATE type);
	AI_STATE getState();

	void setMessageBoard(MessageBoard * messageBoard);
	MessageBoard * getMessageBoard();

	void FindNearbyEntity(Entity * entity);
	void MoveRandomly();

	// Testing AI
	float randomAngle;
	float randomVelocity;
private:
	AI_TYPE m_eType;
	AI_STATE m_eState;
	MessageBoard * m_MessageBoard;
	Entity * m_eTargetEntity;

	double m_dMovementDelay;
	double m_dMovementDelay2;
	double m_dAttackDelay;
};

#endif