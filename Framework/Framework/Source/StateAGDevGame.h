#ifndef STATE_AGDEV_GAME_H
#define STATE_AGDEV_GAME_H

#include "State.h"
#include <string>
#include "MeshBuilder.h"

#include "Camera.h"
#include "EntityPlayer.h"
#include "EntityNPC.h"
#include "EntityWorldObject.h"

#include "LuaReader.h"

class Tree;

#define SPAWN_LIMIT 100

class StateAGDevGame : public State
{
public:
	StateAGDevGame(std::string name, View * theView, int Level = 1) : State(name, theView)
	{ 
		this->theView = theView; 
		m_bStartFadeOut = false;
	}
	~StateAGDevGame();

	State * getInstance();
	void Init();
	void Update(StateHandler * stateHandler, double dt);
	void Cleanup();
	void Pause();
	void Resume();
	void HandleEvents(StateHandler * stateHandler);
	void HandleEvents(StateHandler * stateHandler, const int key, const bool status = true);
	void Draw(StateHandler * stateHandler);

	// Rendering Functions
	void RenderFadeOverlay();
	void RenderSkyplane();
	void RenderGround();
	void RenderPlayer();
	void RenderOctree();
	void RenderBorders();
	void RenderGUI();

	// Fade Effect
	void FadeOutEffect(double dt);

	// Octree
	Tree * tree;
	
	// Entity Spawning
	void SpawnNPCs();
private:
	static StateAGDevGame theGameState;

	Camera * theCamera;
	Entity * thePlayer;

	std::vector<Mesh*> m_meshList;
	std::vector<Entity*> m_entityList;

	int m_iScore;

	View * theView;

	// Fade Effect
	bool m_bStartFadeOut;
};

#endif
