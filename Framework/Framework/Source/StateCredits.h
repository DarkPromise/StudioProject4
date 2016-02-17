#ifndef STATE_CREDITS_H
#define STATE_CREDITS_H

#include "State.h"
#include "MeshBuilder.h"

class StateCredits : public State
{
public:
	StateCredits(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}

	~StateCredits();

	State* getInstance();
	void Init();
	void Update(StateHandler * stateHandler, double dt);
	void Cleanup();
	void Pause();
	void Resume();
	void HandleEvents(StateHandler * stateHandler);
	void HandleEvents(StateHandler * stateHandler, const int key, const bool status = true);
	void Draw(StateHandler * stateHandler);

	// Rendering Functions
	void RenderBackground();

private:
	View * theView;
	std::vector<Mesh*> m_meshList;

};

#endif