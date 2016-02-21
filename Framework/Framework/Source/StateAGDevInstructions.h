#ifndef STATE_AGDEV_INSTRUCTIONS_H
#define STATE_AGDEV_INSTRUCTIONS_H

#include "State.h"
#include "MeshBuilder.h"

#define TEXT_FONT 0

class StateAGDevInstructions : public State
{
public:
	StateAGDevInstructions(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}
	~StateAGDevInstructions();

	State* getInstance();
	void Init();
	void Update(StateHandler * stateHandler, double dt);
	void Cleanup();
	void Pause();
	void Resume();
	void HandleEvents(StateHandler * stateHandler);
	void HandleEvents(StateHandler * stateHandler, const int key, const bool status = true);
	void Draw(StateHandler * stateHandler);

	// State Specific Functions
	void FadeInEffect(double dt);
	void FadeOutEffect(double dt, StateHandler * stateHandler);

	// Rendering Functions
	void RenderBackground();
	void RenderInstructions();
private:
	View * theView;
	std::vector<Mesh*> m_meshList;

	// Fade Effect
	double m_dFadeDelay;
	bool m_bStartFadeIn;
	bool m_bStartFadeOut;
};

#endif