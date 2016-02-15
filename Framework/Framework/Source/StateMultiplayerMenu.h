#ifndef STATE_MULTIPLAYER_MENU_H
#define STATE_MULTIPLAYER_MENU_H

#include "State.h"
#include "GuiButton.h"

class StateHost;
class StateConnect;

#define TEXT_FONT 0

class StateMultiplayerMenu : public State
{
public:
	enum MENU_BUTTONS
	{
		START_BUTTON,
		HOST_BUTTON,
		BACK_BUTTON
	};

	StateMultiplayerMenu(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}

	~StateMultiplayerMenu();

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

	// Rendering Functions
	void RenderBackground();
	void RenderButtons();
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	std::vector<Gui*> m_guiList;
};

#endif