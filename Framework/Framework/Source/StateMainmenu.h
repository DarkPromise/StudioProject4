#ifndef STATE_MAINMENU_H
#define STATE_MAINMENU_H

#pragma warning(disable:4018)

#include "State.h"
#include "GuiButton.h"

#define TEXT_FONT 0

class StateMainmenu : public State
{
public:
	enum MENU_BUTTONS
	{
		AGDEV_BUTTON = 0
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