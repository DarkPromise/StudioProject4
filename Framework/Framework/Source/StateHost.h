#ifndef STATE_HOST_H
#define STATE_HOST_H

#include "State.h"
#include "GuiButton.h"

class StateMultiplayerMenu;
class CharBuffer;

#include "ServerManager.h"
#include "EntityTest.h"

#define TEXT_FONT 0

class StateHost : public State
{
public:
	enum MENU_BUTTONS
	{
		START_BUTTON,
		PORT_BUTTON,
		BACK_BUTTON
	};

	StateHost(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}

	~StateHost();

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

	// Entity Creation
	void createNewEntity();

	// Rendering Functions
	void RenderBackground();
	void RenderButtons();
	void RenderBuffer();
	void RenderEntities();

	// Server Specific
	bool isServerRunning() { return m_bServerRunning; }
	void setServerRunning(bool status) { this->m_bServerRunning = status; }
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	std::vector<Gui*> m_guiList;
	CharBuffer * m_charBuffer;
	ServerManager * m_cServerManager;
	bool m_bServerRunning;
};

#endif