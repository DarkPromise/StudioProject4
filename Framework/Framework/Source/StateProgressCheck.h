#ifndef STATE_PROGRESS_CHECK_H
#define STATE_PROGRESS_CHECK_H

#include "State.h"
#include "MeshBuilder.h"

class StateProgressCheck : public State
{
public:
	StateProgressCheck(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}

	~StateProgressCheck();

	State* getInstance();
	void Init();
	void Update(StateHandler * stateHandler, double dt);
	void Cleanup();
	void Pause();
	void Resume();
	void HandleEvents(StateHandler * stateHandler);
	void HandleEvents(StateHandler * stateHandler, const int key, const bool status = true);
	void Draw(StateHandler * stateHandler);

	// Class Specific Enum
	enum OBJECT_LIST
	{
		OBJECT_AXES = 0,
		OBJECT_TEXT,
		OBJECT_BACKGROUND,
		OBJECT_WALL,
		OBJECT_ENEMY1,
		OBJECT_ENEMY2,
		OBJECT_ENEMY3,
		OBJECT_BASE,
		TOTAL_OBJECTS
	};

	// Class Specific Functions
	void RenderBackground();
	void RenderWall();
	void RenderEnemies();
	void RenderBase();
	void RenderMessageBoard();

private:
	View * theView;
	std::vector<Mesh*> m_meshList;
};

#endif