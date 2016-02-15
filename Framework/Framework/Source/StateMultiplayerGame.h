#ifndef STATE_MULTIPLAYER_GAME
#define STATE_MULTIPLAYER_GAME

#include "State.h"
#include "MeshBuilder.h"

#include "ServerManager.h"
#include "EntityTest.h"

#define TEXT_FONT 0

class StateMultiplayerGame : public State
{
public:
	StateMultiplayerGame(std::string name, View * theView, std::string ipAddress);
	~StateMultiplayerGame();

	State* getInstance();
	void Init();
	void Update(StateHandler * stateHandler, double dt);
	void Cleanup();
	void Pause();
	void Resume();
	void HandleEvents(StateHandler * stateHandler);
	void HandleEvents(StateHandler * stateHandler, const int key, const bool status = true);
	void Draw(StateHandler * stateHandler);

	// Entity Creation (HARDCODED!!!)
	void createNewEntity();
	Entity * getOtherPlayer() { return this->otherPlayer; }

	// Rendering Functions
	void RenderBackground();
	void RenderPlayer();
	void RenderEntities();

	// Testing for now
	double pingDelay;
	int otherPlayerIndex;
	char** splitText(char *string, char *delim, int *count);
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	std::vector<Entity*> m_entityList;
	ServerManager * m_cServerManager;
	Entity * player;
	Entity * otherPlayer;
};

#endif