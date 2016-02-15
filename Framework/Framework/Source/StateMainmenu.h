#ifndef STATE_MAINMENU_H
#define STATE_MAINMENU_H

#include "State.h"
#include "GuiButton.h"

class StateMainmenu : public State
{
public:
	enum MENU_BUTTONS
	{
		MULTIPLAYER_BUTTON = 0,
		AI_BUTTON,
		AGDEV_BUTTON,
	};

	StateMainmenu(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}

	~StateMainmenu();

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
	void UpdateSelection(StateHandler * stateHandler);
	void FadeOutEffect(double dt, StateHandler * stateHandler);

	// Rendering Functions
	void RenderBackground();
	void RenderButtons();
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	std::vector<Gui*> m_guiList;

	// Fade Effect
	double m_dFadeDelay;
	bool m_bStartFade;
};

#endif