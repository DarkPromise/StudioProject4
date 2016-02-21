#include "StateAGDevGame.h"
#include "View.h"
#include "LoadTGA.h"
#include "Tree.h"

#include "StateAGDevMenu.h"
#include <sstream>

#define DOMAIN_SIZE 1000.f
#define WORLD_SIZE Vector3(2000.f,2000.f,2000.f)

StateAGDevGame::~StateAGDevGame()
{

}

State * StateAGDevGame::getInstance()
{
	return this;
}

void StateAGDevGame::Init()
{
	// Init RNG
	Math::InitRNG();

	// Menu in 2D
	theView->LoadPerspectiveCamera(90.f);

	// Create Meshes
	Mesh * newMesh;
	newMesh = MeshBuilder::GenerateText("Source Font", 16, 16);
	newMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Fade Overlay", Color(0.f, 0.f, 0.f), 1.f);
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateSkyPlane("Sky", Color(0.f, 0.f, 0.f), 128, 1000.0f, 7500.f, 1.f, 1.f);
	newMesh->textureArray[0] = LoadTGA("Images//Sky.tga");
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Ground", Color(0.f, 0.f, 0.f), 15000.f);
	newMesh->textureArray[0] = LoadTGA("Images//Grass.tga");
	m_meshList.push_back(newMesh);

	// Init the Camera
	theCamera = new Camera();
	theView->getInputHandler()->resetMousePosition(theView);
	theView->getInputHandler()->setMouseEnabled(false);

	// Init the Score
	m_iScore = 0;

	// Create Octree (Can use Lua to Initialize Variables)
	LuaReader octreeScript("Scripts//Octree.lua");
	int treeLevels = octreeScript.get<int>("Octree.levels");
	Vector3 treeCenter = octreeScript.get<Vector3>("Octree.position");
	tree = new Tree();
	tree->createTree(treeLevels , treeCenter , WORLD_SIZE);

	// Create Player Entity (Using Lua!!!)
	LuaReader playerScript("Scripts//Player.lua");
	thePlayer = playerScript.createEntity("Player", theCamera);
	tree->addObject(thePlayer);

	// Create Entities **NOTE : the Octree will render the entities
	for (int i = 0; i < 20; i++)
	{
		float randomNPC = Math::RandIntMinMax(1, 100); // Random Chance to spawn a neutral or  hostile NPC

		Entity * newNpc = new EntityNPC();
		auto * infoComponent = new InformationComponent();
		infoComponent->setName("NPC");
		infoComponent->setPosition(Vector3(Math::RandFloatMinMax(-DOMAIN_SIZE, DOMAIN_SIZE), 0.f, Math::RandFloatMinMax(-DOMAIN_SIZE, DOMAIN_SIZE)));
		infoComponent->setType(InformationComponent::ENTITY_TYPE::TYPE_NPC);
		infoComponent->setSize(10.f);
		newNpc->addComponent(infoComponent);
		auto * graphicsComponent = new GraphicsComponent();
		if (randomNPC <= 20) // 20% Chance
		{
			graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (LOW RES)", Color(0.6f, 0.f, 0.f), 5 ,9, 5.f));
			graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (MED RES)", Color(0.8f, 0.f, 0.f), 8 ,12, 5.f));
			graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (HIGH RES)", Color(1.f, 0.f, 0.f), 14 ,18, 5.f));
		}
		else
		{
			graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (LOW RES)", Color(0.f, 0.6f, 0.f), 5, 9, 5.f));
			graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (MES RES)", Color(0.f, 0.8f, 0.f), 8, 12, 5.f));
			graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (HIGH RES)", Color(0.f, 1.f, 0.f), 14, 18, 5.f));
		}
		newNpc->addComponent(graphicsComponent);
		auto * collisionComponent = new CollisionComponent();
		collisionComponent->setMinMax(Vector3(-5.f, -5.f, -5.f), Vector3(5.f, 5.f, 5.f));
		collisionComponent->setMesh(MeshBuilder::GenerateBoundingBox("NPCBB", collisionComponent->getAABB()->Max, collisionComponent->getAABB()->Min, Color(0.f, 1.f, 0.f)));
		newNpc->addComponent(collisionComponent);
		auto * aiComponent = new AIComponent();
		if (randomNPC <= 20) // 20% Chance
		{
			aiComponent->setType(AIComponent::AI_ENEMY);
			aiComponent->setState(AIComponent::STATE_SEARCH);
		}
		else
		{
			aiComponent->setType(AIComponent::AI_NEUTRAL);
			aiComponent->setState(AIComponent::STATE_IDLE);
		}
		newNpc->addComponent(aiComponent);
		m_entityList.push_back(newNpc);
		tree->addObject(newNpc);
	}

	m_bStartFadeOut = true;
}

void StateAGDevGame::Update(StateHandler * stateHandler, double dt)
{
	// Spawn Random NPCs
	static double spawnDelay = 0.0;
	spawnDelay += dt;
	if (spawnDelay > 2.0) // Spawn every 2s
	{
		if (m_entityList.size() < SPAWN_LIMIT)
		{
			SpawnNPCs();
		}
		spawnDelay = 0.0;
	}

	// Player Update
	auto infoC = thePlayer->getComponent<InformationComponent>();
	if (infoC)
	{
		infoC->Update(dt);
	}
	
	// Entities Update
	for (Entity * entity : m_entityList)
	{
		auto infoC = entity->getComponent<InformationComponent>();
		if (infoC)
		{
			infoC->Update(dt);
		}
	}

	// Camera Update
	theCamera->Update(dt, theView->getInputHandler());
	auto cameraC = thePlayer->getComponent<CameraComponent>();
	if (cameraC)
	{
		cameraC->Update(dt);
	}

	// Octree Update
	tree->UpdateAIComponents(dt, thePlayer);
	tree->UpdateLevelOfDetails(theCamera->getCameraPos());
	int temp1, temp2;
	tree->checkCollisions(temp1, temp2, m_iScore);

	if (m_bStartFadeOut)
	{
		FadeOutEffect(dt);
	}
	theView->Update(dt);

	theView->viewStack.LoadIdentity();
	theView->viewStack.LookAt(
		theCamera->getCameraPos().x, theCamera->getCameraPos().y, theCamera->getCameraPos().z,
		theCamera->getCameraTarget().x, theCamera->getCameraTarget().y, theCamera->getCameraTarget().z,
		theCamera->getCameraUp().x, theCamera->getCameraUp().y, theCamera->getCameraUp().z
		);
}

void StateAGDevGame::HandleEvents(StateHandler * stateHandler)
{
	// Should be done with "ControllerComponent"
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_W))
	{
		thePlayer->getComponent<InformationComponent>()->setVelocity(MOVE_SPEED);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_S))
	{
		thePlayer->getComponent<InformationComponent>()->setVelocity(-MOVE_SPEED);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_A))
	{
		thePlayer->getComponent<InformationComponent>()->applyRotation(ROTATION_SPEED, 1);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_D))
	{
		thePlayer->getComponent<InformationComponent>()->applyRotation(-ROTATION_SPEED, 1);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_SPACE))
	{
		auto playerEntity = dynamic_cast<EntityPlayer*>(thePlayer);
		playerEntity->Jump();
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE))
	{
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, false));
	}

	if (thePlayer)
	{
		auto healthC = thePlayer->getComponent<HealthComponent>();
		if (healthC)
		{
			if (healthC->getHealth() <= 0)
			{
				stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, false));
			}
		}
	}
}

void StateAGDevGame::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{
	// This Function is Obsolete
}

void StateAGDevGame::Cleanup()
{
	if (thePlayer)
	{
		delete thePlayer;
	}

	if (theCamera)
	{
		delete theCamera;
	}

	if (tree)
	{
		delete tree;
	}

	if (!m_entityList.empty())
	{
		for (int i = 0; i < m_entityList.size(); i++)
		{
			delete m_entityList[i];
		}
	}
	m_entityList.clear();
	m_entityList.~vector();

	if (!m_meshList.empty())
	{
		for (int i = 0; i < m_meshList.size(); i++)
		{
			delete m_meshList[i];
		}
	}
	m_meshList.clear();
	m_meshList.~vector(); // There will be a memory leak if you dont deallocate the memory given to the vector itself
}

void StateAGDevGame::Pause()
{

}

void StateAGDevGame::Resume()
{

}

void StateAGDevGame::Draw(StateHandler * stateHandler)
{
	RenderSkyplane();
	RenderGround();
	RenderOctree();
	//RenderPlayer(); Now Obsolete because the Octree Renders all Entities
	RenderGUI();
	RenderFadeOverlay();
	theView->SwapBuffers();
}

void StateAGDevGame::RenderFadeOverlay()
{
	if (m_meshList[1]->alpha > 0)
	{
		theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	}
}

void StateAGDevGame::RenderSkyplane()
{
	theView->modelStack.PushMatrix();
	theView->modelStack.Translate(500, 5000, -500);
	theView->RenderMesh(m_meshList[2], false, false);
	theView->modelStack.PopMatrix();
}

void StateAGDevGame::RenderGround()
{
	theView->modelStack.PushMatrix();
	theView->modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	theView->modelStack.Rotate(-90.f, 0.f, 0.f, 1.f);
	theView->RenderMesh(m_meshList[3], false, false);
	theView->modelStack.PopMatrix();
}

void StateAGDevGame::RenderPlayer()
{
	theView->modelStack.PushMatrix();
	theView->modelStack.Translate(this->thePlayer->getComponent<InformationComponent>()->getPosition().x, this->thePlayer->getComponent<InformationComponent>()->getPosition().y, this->thePlayer->getComponent<InformationComponent>()->getPosition().z);
	theView->modelStack.Rotate(this->thePlayer->getComponent<InformationComponent>()->getRotation().y, 0.f, 1.f, 0.f);
	theView->RenderMesh(this->thePlayer->getComponent<GraphicsComponent>()->getMesh(this->thePlayer->getComponent<GraphicsComponent>()->getResLevel()), false, false);
	theView->modelStack.PopMatrix();

	auto collisionC = this->thePlayer->getComponent<CollisionComponent>();
	if (collisionC)
	{
		theView->modelStack.PushMatrix();
		theView->modelStack.Translate(this->thePlayer->getComponent<InformationComponent>()->getPosition().x, this->thePlayer->getComponent<InformationComponent>()->getPosition().y, this->thePlayer->getComponent<InformationComponent>()->getPosition().z);
		theView->modelStack.Rotate(this->thePlayer->getComponent<InformationComponent>()->getRotation().y, 0.f, 1.f, 0.f);
		theView->RenderMesh(collisionC->getMesh(), false, false);
		theView->modelStack.PopMatrix();
	}
}

void StateAGDevGame::RenderOctree()
{
	glLineWidth(3.0f);
	tree->Render(theView);
	glLineWidth(1.0f);
}

void StateAGDevGame::RenderGUI()
{
	std::stringstream ss1;
	ss1.precision(0);
	std::stringstream ss2;
	ss2.precision(0);
	std::stringstream ss3;
	ss3.precision(0);

	ss1 << "Hp[" << thePlayer->getComponent<HealthComponent>()->getHealth() << "/" << thePlayer->getComponent<HealthComponent>()->getMaxHealth() << "]";
	theView->RenderTextOnScreen(m_meshList[0], ss1.str(), Color(1.f, 1.f, 1.f), 48.f, 5.f, 0.f);

	// This was just a test
	ss2 << "Ammo[" << thePlayer->getComponent<InformationComponent>()->getAmmo() << "/" << thePlayer->getComponent<InformationComponent>()->getMaxAmmo() << "]";
	//theView->RenderTextOnScreen(m_meshList[0], ss2.str(), Color(1.f, 1.f, 1.f), 48.f, this->theView->getWindowWidth() - (ss2.str().length() * 20.f), 0.f);

	ss3 << "Score : " << m_iScore;
	theView->RenderTextOnScreen(m_meshList[0], ss3.str(), Color(1.f, 1.f, 1.f), 48.f, (this->theView->getWindowWidth() * 0.5f) - (ss3.str().length() * 10.f), 0.f);
}

void StateAGDevGame::FadeOutEffect(double dt)
{
	if (m_meshList[1]->alpha > 0)
	{
		if (dt < 0.3)
		{
			m_meshList[1]->alpha -= 2.f * dt;
		}
	}
	else
	{
		m_bStartFadeOut = false;
	}
}

void StateAGDevGame::SpawnNPCs()
{
	float randomNPC = Math::RandIntMinMax(1, 100); // Random Chance to spawn a neutral or  hostile NPC

	Entity * newNpc = new EntityNPC();
	auto * infoComponent = new InformationComponent();
	infoComponent->setName("NPC");
	infoComponent->setPosition(Vector3(Math::RandFloatMinMax(-DOMAIN_SIZE, DOMAIN_SIZE), Math::RandFloatMinMax(1000.f,2000.f), Math::RandFloatMinMax(-DOMAIN_SIZE, DOMAIN_SIZE)));
	infoComponent->setType(InformationComponent::ENTITY_TYPE::TYPE_NPC);
	infoComponent->setSize(10.f);
	newNpc->addComponent(infoComponent);
	auto * graphicsComponent = new GraphicsComponent();
	if (randomNPC <= 20) // 20% Chance
	{
		graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (LOW RES)", Color(0.6f, 0.f, 0.f), 5, 9, 5.f));
		graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (MED RES)", Color(0.8f, 0.f, 0.f), 8, 12, 5.f));
		graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (HIGH RES)", Color(1.f, 0.f, 0.f), 14, 18, 5.f));
	}
	else if (randomNPC < 50)
	{
		graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (LOW RES)", Color(0.f, 0.6f, 0.f), 5, 9, 5.f));
		graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (MES RES)", Color(0.f, 0.8f, 0.f), 8, 12, 5.f));
		graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (HIGH RES)", Color(0.f, 1.f, 0.f), 14, 18, 5.f));
	}
	else
	{
		graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (LOW RES)", Color(0.3f, 0.4f, 0.5f), 5, 9, 5.f));
		graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (MES RES)", Color(0.5f, 0.6f, 0.7f), 8, 12, 5.f));
		graphicsComponent->addMesh(MeshBuilder::GenerateSphere("NPC (HIGH RES)", Color(0.7f, 0.8f, 0.9f), 14, 18, 5.f));
	}
	newNpc->addComponent(graphicsComponent);
	auto * collisionComponent = new CollisionComponent();
	collisionComponent->setMinMax(Vector3(-5.f, -5.f, -5.f), Vector3(5.f, 5.f, 5.f));
	collisionComponent->setMesh(MeshBuilder::GenerateBoundingBox("NPCBB", collisionComponent->getAABB()->Max, collisionComponent->getAABB()->Min, Color(0.f, 1.f, 0.f)));
	newNpc->addComponent(collisionComponent);
	auto * aiComponent = new AIComponent();
	if (randomNPC <= 20) // 20% Chance
	{
		aiComponent->setType(AIComponent::AI_ENEMY);
		aiComponent->setState(AIComponent::STATE_SEARCH);
	}
	else if (randomNPC < 50)
	{
		aiComponent->setType(AIComponent::AI_NEUTRAL);
		aiComponent->setState(AIComponent::STATE_IDLE);
	}
	else
	{
		aiComponent->setType(AIComponent::AI_SUPPORT);
		aiComponent->setState(AIComponent::STATE_IDLE);
	}
	newNpc->addComponent(aiComponent);
	m_entityList.push_back(newNpc);
	tree->addObject(newNpc);
}