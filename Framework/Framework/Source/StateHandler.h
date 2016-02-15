#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include <iostream>
#include <vector>

class State;

class StateHandler
{
public:
	StateHandler();
	virtual ~StateHandler();

	virtual void Init();
	virtual void ChangeState(State * state);
	virtual void PushState(State * state);
	virtual void PopState();
	virtual void HandleEvents();
	virtual void HandleEvents(const int key, const bool status = true);
	virtual void Cleanup();
	virtual void Draw();
	virtual void Update(double m_dElaspedTime);
private:
	std::vector<State*> m_stateList;
	std::vector<State*> m_deleteList;
	bool m_bRunning;
};

#endif