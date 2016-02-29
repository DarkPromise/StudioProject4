#ifndef CONTROLLER_COMPONENT_H
#define CONTROLLER_COMPONENT_H

#pragma warning(disable:4101)

#include "Component.h"

#include "InputHandler.h"

#include "GridMap.h"

#define MOVEMENT_DELAY 0.15
#define INTERACTION_DELAY 0.2

struct MovementUpdate
{
	MovementUpdate(int indexX, int indexY, GridMap::GRID_DIRECTION direction = GridMap::DIRECTION_NONE, Vector3 nextPos = Vector3(0.f, 0.f, 0.f), bool isMoving = false)
	{
		m_iPlayerIndexX = indexX;
		m_iPlayerIndexY = indexY;
		m_eMoveDirection = direction;
		m_v3NextPosition = nextPos;
		m_bIsMoving = isMoving;
	}

	GridMap::GRID_DIRECTION m_eMoveDirection;
	bool m_bIsMoving;
	Vector3 m_v3NextPosition;
	int m_iPlayerIndexX;
	int m_iPlayerIndexY;
};

class ControllerComponent : public Component
{
public:
	ControllerComponent(InputHandler * theInputHandler);
	virtual ~ControllerComponent();

	// Generic Functions
	void CreateComponent(luabridge::LuaRef & tableInfo, std::string name);
	virtual void Update(double dt,GridMap * currMap);
	virtual void MoveForward(GridMap * currMap);
	virtual void MoveBackwards(GridMap * currMap);
	virtual void MoveLeft(GridMap * currMap);
	virtual void MoveRight(GridMap * currMap);
	virtual void Interact(GridMap * currMap);
public:
	void setInputHandler(InputHandler * inputHandler);
	InputHandler * getInputHandler();

	void setInputDelay(double delay);
	double getInputDelay();

	MovementUpdate getMovementStruct() { return m_sMovement; }
private:
	InputHandler * m_cInputHandler;
	double m_dInputDelay;
	double m_dMoveDelay;
	MovementUpdate m_sMovement;
};

#endif