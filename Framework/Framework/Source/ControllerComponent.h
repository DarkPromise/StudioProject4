#ifndef CONTROLLER_COMPONENT_H
#define CONTROLLER_COMPONENT_H

#include "Component.h"

#include "InputHandler.h"

#include "GridMap.h"

#define MOVEMENT_DELAY 0.15
#define INTERACTION_DELAY 0.2

class ControllerComponent : public Component
{
public:
	ControllerComponent(InputHandler * theInputHandler);
	virtual ~ControllerComponent();

	// Generic Functions
	void CreateComponent(luabridge::LuaRef & tableInfo);
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
private:
	InputHandler * m_cInputHandler;
	double m_dInputDelay;
};

#endif