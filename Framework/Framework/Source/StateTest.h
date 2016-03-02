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
extern 	CRecord playerRecord;
class StateTest : public State
{
public:
	StateTest(std::string name, View * theView, int gameType, int levelSelected)
		: State(name, theView)
		, moveDelay(0.f)
		, gameTimer(0)
		, state(STATE_PLAY)
		, gameSaved(false)
		, gameSavedTimer(0)
		, showLegend(false)
	{
		this->theView = theView;
		this->gameType = static_cast<GAMETYPE>(gameType);
		this->levelSelected = levelSelected;
	}

	enum GAMESTATE
	{
		STATE_PLAY,
		STATE_PAUSE,
		STATE_GAMEOVER
	};

	enum GAMETYPE
	{
		GAMETYPE_NEWGAME = 1,
		GAMETYPE_LOADGAME,
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
	void resetAllEntityCount();

	// Rendering Functions
	void RenderPlayer();
	void RenderGUI();
	void RenderAI();
	void RenderBackground();
	void RenderFadeQuad();
	void RenderLegend();

	// Fade Functions
	void RestartFade();
	void FadeInEffect(double dt);
	void FadeOutEffect(double dt);

	// Game Functions
	void RestartLevel();
	void SetGlobalDifficulty(int i);
	int GetGlobalDifficulty();

	// Saving 
	void gameSave(GameplayComponent *gameC, InformationComponent *infoC, HealthComponent *playerHealth);
	void loadPlayer(GridMap *testMap, InformationComponent *informationComponent, GameplayComponent *gameC, HealthComponent *playerHealth);
	void loadLevel1(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, GAMETYPE gameType);
	void loadLevel2(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, GAMETYPE gameType);
	void loadLevel3(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, GAMETYPE gameType);
	void loadLevel4(GridMap * testMap, GraphicsComponent * graphicsComponent, EntityGridObject * gridObject, GameplayComponent * gameC, GAMETYPE gameType);

	GridMap * testMap;
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	Camera * theCamera;
	Entity * testEntity;
	std::vector<Entity*> m_guardList;
	EntityGridObject *testGridObject;
	float moveDelay;
	float gameTimer;
	GAMESTATE state;
	bool gameSaved;
	float gameSavedTimer;
	GAMETYPE gameType;
	int levelSelected;
	bool showLegend;

	// Fading Effect
	double m_dFadeDelay;
	bool m_bStartFadeIn;
	bool m_bStartFadeOut;
	bool alpha;
	bool runOnce;
};

#endif