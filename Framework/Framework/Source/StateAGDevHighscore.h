#ifndef STATE_AGDEV_HIGHSCORE_H
#define STATE_AGDEV_HIGHSCORE_H

#include "State.h"
#include "MeshBuilder.h"
#include "Highscore.h"

#include <string>
#include <fstream>
#include <sstream>
#define TEXT_FONT 0

class StateAGDevHighscore : public State
{
public:
	StateAGDevHighscore(std::string name, View * theView) : State(name, theView), tempName("     ")
	{
		this->theView = theView;
	}
	~StateAGDevHighscore();

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
	void RenderHighscore();

	// Highscore
	//std::string m_sCurrHighscore;
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	Highscore highscore;
	std::string tempName;

	// Fade Effect
	double m_dFadeDelay;
	bool m_bStartFadeIn;
	bool m_bStartFadeOut;
};

#endif