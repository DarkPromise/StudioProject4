#ifndef STATE_H
#define STATE_H

#pragma warning(disable:4244)

#include "StateHandler.h"
#include <string>

class View;

class State
{
public:
	State(std::string name, View * theView) : m_sName(name), theView(theView)
	{

	}

	virtual ~State(){}

	virtual State* getInstance() = 0;
	virtual void Init() = 0;
	virtual void Update(StateHandler * stateHandler, double dt) = 0;
	virtual void Cleanup() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void HandleEvents(StateHandler * stateHandler) = 0;
	virtual void HandleEvents(StateHandler * stateHandler, const int key, const bool status = true) = 0;
	virtual void Draw(StateHandler * stateHandler) = 0;

	std::string toString()
	{
		return this->m_sName;
	}

	void ChangeState(StateHandler * stateHandler, State * state)
	{
		stateHandler->ChangeState(state);
	}
private:
	std::string m_sName;
	View * theView;
};

#endif