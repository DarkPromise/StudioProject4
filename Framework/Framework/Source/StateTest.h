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
#include "Highscore.h"

class StateTest : public State
{
public:
	StateTest(std::string name, View * theView, int gameType) 
		: State(name, theView)
		, moveDelay(0.f)
		, gameTimer(0)
		, state(STATE_PLAY)
		, gameSaved(false)
		, gameSavedTimer(0)
		, gameType(gameType)
	{
		this->theView = theView;
	}

	enum GAMESTATE
	{
		STATE_PLAY,
		STATE_PAUSE,
		STATE_GAMEOVER
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

	// Rendering Functions
	void RenderPlayer();
	void RenderGUI();
	void RenderAI();

	// Saving 
	void gameSave(InformationComponent *infoC);
	void loadPlayer(GridMap *testMap, InformationComponent *informationComponent, GameplayComponent *gameC);
	void loadLevel1(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, int gameType);
	void loadLevel2(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, int gameType);
	void loadLevel3(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, int gameType);

	GridMap * testMap;
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	Camera * theCamera;
	Entity * testEntity;
	Entity * testGuard;
	EntityGridObject *testGridObject;
	float moveDelay;
	float gameTimer;
	GAMESTATE state;
	bool gameSaved;
	float gameSavedTimer;
	Highscore highscore;
	CRecord playerRecord;
	int gameType;
};

#endif