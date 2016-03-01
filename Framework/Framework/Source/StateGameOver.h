#ifndef STATE_GAMEOVER_H
#define STATE_GAMEOVER_H

#include "State.h"
#include "MeshBuilder.h"
#include <sstream>
#include "Highscore.h"
class StateGameOver : public State
{
public:
	StateGameOver(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}
	
	~StateGameOver();

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
	void RenderNames();
	void RenderHighscore();

private:
	enum GAMESTATE
	{
		GETTINGPLAYERNAME,
		MAINMENU
	};
	View * theView;
	std::vector<Mesh*> m_meshList;

	// Fade Effect
	double m_dFadeDelay;
	bool m_bStartFadeIn;
	bool m_bStartFadeOut;
	Highscore highscore;
	bool chara[32];
	int currentLetter;
	GAMESTATE gamestate;
	std::string tempName;
};

#endif