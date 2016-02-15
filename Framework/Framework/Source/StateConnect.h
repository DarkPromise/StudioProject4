#ifndef STATE_CONNECT_H
#define STATE_CONNECT_H

#include "State.h"
#include "GuiButton.h"

class StateMultiplayerMenu;
class CharBuffer;

#define TEXT_FONT 0

class StateConnect : public State
{
public:
	enum MENU_BUTTONS
	{
		START_BUTTON,
		IP_BUTTON,
		BACK_BUTTON
	};

	StateConnect(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}

	~StateConnect();

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
	CharBuffer * getBuffer();

	// Rendering Functions
	void RenderBackground();
	void RenderButtons();
	void RenderBuffer();
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	std::vector<Gui*> m_guiList;
	CharBuffer * m_charBuffer;
};

#endif