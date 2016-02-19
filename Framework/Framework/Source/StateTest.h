#ifndef STATE_TEST_H
#define STATE_TEST_H
#define TEXT_FONT 0

#include "State.h"
#include "MeshBuilder.h"
#include "EntityTest.h"
#include "EntityGridObject.h"
#include "Camera.h"
#include "GridMap.h"
#include "LuaReader.h"
#include "SoundManager.h"

class StateTest : public State
{
public:
	StateTest(std::string name, View * theView) 
		: State(name, theView)
		, moveDelay(0.f)
		, gameTimer(0)
		, state(STATE_PLAY)
		, gameSaved(false)
		, gameSavedTimer(0)
		, level(1)
	{
		this->theView = theView;
	}

	enum GAMESTATE
	{
		STATE_PLAY,
		STATE_PAUSE
	};

	State* getInstance();
	void Init();
	void Update(StateHandler * stateHandler, double dt);
	void Cleanup();
	void Pause();
	void Resume();
	void HandleEvents(StateHandler * stateHandler);
	void HandleEvents(StateHandler * stateHandler, const int key, const bool status = true);
	void Draw(StateHandler * stateHandler);
	void renderPlayer();
	void renderGUI();

	GridMap * testMap;
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	Camera * theCamera;
	Entity * testEntity;
	float moveDelay;
	float gameTimer;
	GAMESTATE state;
	bool gameSaved;
	float gameSavedTimer;
	int level;
};

#endif