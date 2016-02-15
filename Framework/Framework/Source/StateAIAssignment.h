#ifndef STATE_AI_ASSIGNMENT_H
#define STATE_AI_ASSIGNMENT_H

#include "State.h"

#include "EntityHero.h"
#include "EntityWall.h"

#include "MessageBoard.h"

class StateAIAssignment : public State
{
public:
	StateAIAssignment(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}

	~StateAIAssignment();

	State* getInstance();
	void Init();
	void Update(StateHandler * stateHandler, double dt);
	void Cleanup();
	void Pause();
	void Resume();
	void HandleEvents(StateHandler * stateHandler);
	void HandleEvents(StateHandler * stateHandler, const int key, const bool status = true);
	void Draw(StateHandler * stateHandler);

	// Class Specific Functions
	void RenderBackground();
	void RenderWall();
	void RenderEnemies();
	void RenderBase();
	void RenderEntities();
	void RenderWaypoints();
	void RenderMessageBoard();

private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	std::vector<Entity*> m_entityList;
	std::vector<Waypoint*> m_waypointList;
	MessageBoard * m_MessageBoard;
};

#endif