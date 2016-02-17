#ifndef STATE_TEST_H
#define STATE_TEST_H

#include "State.h"
#include "MeshBuilder.h"

#include "EntityTest.h"
#include "EntityGridObject.h"

#include "Camera.h"

#include "GridMap.h"

class StateTest : public State
{
public:
	StateTest(std::string name, View * theView) 
		: State(name, theView)
		, moveDelay(0.f)
	{
		this->theView = theView;
	}

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

	GridMap * testMap;
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	Camera * theCamera;
	Entity * testEntity;
	float moveDelay;
};

#endif