#include "StateTest.h"
#include "View.h"
#include "StateAGDevMenu.h"
#include "StateAGDevOptions.h"

#define HUD_DISPLAY_DELAY	3
#define xSize				32
#define ySize				25
#define NUM_BOXES			3
#define NUM_SWITCHES		1

State * StateTest::getInstance()
{
	return this;
}

void StateTest::Init()
{
	// Testing will be done in Perspective
	theView->LoadPerspectiveCamera(90.0);
	
	// MESHES
	Mesh * testMesh;
	testMesh = MeshBuilder::GenerateText("Source Font", 16, 16);
	testMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(testMesh);

	// SOUND
	if (SoundManager::getSoundStatus())
	{
		SoundManager::playSound("Sounds//movesfx.wav", false);
	}

	theCamera = new Camera();
	theView->getInputHandler()->resetMousePosition(theView);
	theView->getInputHandler()->setMouseEnabled(false);

	testMap = new GridMap();
	testMap->Init(xSize, ySize);
	
	// PLAYER
	LuaReader playerScript("Scripts//Player.lua");
	testEntity = playerScript.createEntity("Player",theCamera,theView->getInputHandler(),testMap);
	auto informationComponent = testEntity->getComponent<InformationComponent>();
	auto gameplayComponent = testEntity->getComponent<GameplayComponent>();
	auto graphicsComponent = testEntity->getComponent<GraphicsComponent>();
	auto gameC = testEntity->getComponent<GameplayComponent>();
	
	// Test AI Guard
	LuaReader guardScript("Scripts//Guard.lua");
	testGuard = guardScript.createEntity("Guard",theCamera,theView->getInputHandler(),testMap);

	switch (gameType)
	{
		case 1:
			informationComponent->setPosition(testMap->getGridMap()[23][1]->getGridPos());
		break;
		case 2:
			loadPlayer(testMap, informationComponent, gameC);
		break;
	}

	// INTIALISE LEVELS
	switch (gameC->getCurrLevel())
	{		
		case 1:
			loadLevel1(testMap, graphicsComponent, testGridObject, gameC, gameType);
		break;
		case 2:
			loadLevel2(testMap, graphicsComponent, testGridObject, gameC, gameType);		
		break;
		case 3:
			loadLevel3(testMap, graphicsComponent, testGridObject, gameC, gameType);
		break;
	}
}

void StateTest::Update(StateHandler * stateHandler, double dt)
{
	static bool click = false;
	auto controlC = testEntity->getComponent<ControllerComponent>();
	auto infoC = testEntity->getComponent<InformationComponent>();
	auto gameC = testEntity->getComponent<GameplayComponent>();
	auto graphicsComponent = testEntity->getComponent<GraphicsComponent>();

	if (testGuard->getComponent<AIComponent>())
	{
		testGuard->getComponent<AIComponent>()->Update(dt, testMap, testEntity);
	}

	// LEVEL CLEARED
	if (gameC)
	{
		if (gameC->getLevelCleared())
		{
			gameC->incrementLevel();
			gameC->setLevelCleared(false);
			gameC->setHasKey(false);

			testMap->ResetData();
			testMap->Init(xSize, ySize);
			switch (gameC->getCurrLevel())
			{
				case 2:					
					loadLevel2(testMap, graphicsComponent, testGridObject, gameC, gameType);
				break;

				case 3:
					loadLevel3(testMap, graphicsComponent, testGridObject, gameC, gameType);
				break;
			}
		}
	}

	// GAME RUNNING
	if (state == GAMESTATE::STATE_PLAY)
	{
		// PAUSE GAME
		if (!click && controlC->getInputHandler()->IsKeyPressed(GLFW_KEY_P))
		{
			click = true;
			state = GAMESTATE::STATE_PAUSE;
		}

		else if (click && !controlC->getInputHandler()->IsKeyPressed(GLFW_KEY_P))
		{
			click = false;
		}

		// SAVE GAME
		if (!click && controlC->getInputHandler()->IsKeyPressed(GLFW_KEY_S))
		{
			click = true;
			gameSaved = true;
			gameSave(infoC);
		}

		else if (click && !controlC->getInputHandler()->IsKeyPressed(GLFW_KEY_S))
		{
			click = false;
		}

		// SHOW GAME SAVING
		if (gameSaved)
		{
			gameTimer += dt;
			if (gameTimer >= HUD_DISPLAY_DELAY)
			{
				gameTimer = 0;
				gameSaved = false;
			}
		}

		// SHOW PLAYER KEY REQUIRED
		if (gameC)
		{
			if (gameC->getKeyShowTimer() > 0)
			{
				gameC->setKeyShowTimer(gameC->getKeyShowTimer() - dt);
			}
		}

		// UPDATE CAMERA
		theCamera->Update(dt,theView->getInputHandler());
		auto cameraC = testEntity->getComponent<CameraComponent>();
		if (cameraC)
		{
			cameraC->Update(dt);
		}
	
		// PLAYER UPDATE
		testEntity->getComponent<GraphicsComponent>()->getMesh()->alpha += dt;
		if (testEntity->getComponent<GraphicsComponent>()->getMesh()->alpha > 2)
		{
			testEntity->getComponent<GraphicsComponent>()->getMesh()->alpha = 0.f;
		}

		if (testEntity)
		{
			if (infoC)
			{
				infoC->Update(dt);
			}
			
			if (controlC)
			{
				controlC->Update(dt,testMap);
			}
		}

		// UPDATE VIEW
		theView->Update(dt);
		theView->viewStack.LoadIdentity();
		theView->viewStack.LookAt(theCamera->getCameraPos().x, theCamera->getCameraPos().y, theCamera->getCameraPos().z,
								  theCamera->getCameraTarget().x, theCamera->getCameraTarget().y, theCamera->getCameraTarget().z,
								  theCamera->getCameraUp().x, theCamera->getCameraUp().y, theCamera->getCameraUp().z);

		gameTimer += dt;
	}

	// GAME PAUSED
	else
	{
		// RESUME GAME
		if (!click && controlC->getInputHandler()->IsKeyPressed(GLFW_KEY_P))
		{
			click = true;
			state = GAMESTATE::STATE_PLAY;
		}

		else if (click && !controlC->getInputHandler()->IsKeyPressed(GLFW_KEY_P))
		{
			click = false;
		}
	}
}

void StateTest::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE))
	{
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, true));
	}
}

void StateTest::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateTest::Cleanup()
{
	if (testGuard)
	{
		delete testGuard;
	}

	if (testEntity)
	{
		delete testEntity;
	}

	if (testMap)
	{
		delete testMap;
	}

	if (theCamera)
	{
		delete theCamera;
	}

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

void StateTest::Pause()
{

}

void StateTest::Resume()
{

}

void StateTest::RenderPlayer()
{
	theView->modelStack.PushMatrix();
	theView->modelStack.Translate(this->testEntity->getComponent<InformationComponent>()->getPosition().x, this->testEntity->getComponent<InformationComponent>()->getPosition().y, this->testEntity->getComponent<InformationComponent>()->getPosition().z);
	theView->modelStack.Rotate(this->testEntity->getComponent<InformationComponent>()->getRotation().y, 0.f, 1.f, 0.f);
	theView->RenderMesh(this->testEntity->getComponent<GraphicsComponent>()->getMesh(this->testEntity->getComponent<GraphicsComponent>()->getResLevel()), false, false);
	theView->modelStack.PopMatrix();

	/*auto collisionC = this->testEntity->getComponent<CollisionComponent>();
	if (collisionC)
	{
		theView->modelStack.PushMatrix();
		theView->modelStack.Translate(this->testEntity->getComponent<InformationComponent>()->getPosition().x, this->testEntity->getComponent<InformationComponent>()->getPosition().y, this->testEntity->getComponent<InformationComponent>()->getPosition().z);
		theView->modelStack.Rotate(this->testEntity->getComponent<InformationComponent>()->getRotation().y, 0.f, 1.f, 0.f);
		theView->RenderMesh(collisionC->getMesh(), false, false);
		theView->modelStack.PopMatrix();
	}*/
}

void StateTest::RenderGUI()
{
	// GAME TIMER
	std::ostringstream ss;
	if (gameTimer < 10)
	{
		ss.precision(3);
	}
	else
	{
		ss.precision(4);
	}
	ss << "TIME: " << gameTimer;
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], ss.str(), Color(1.f, 0.f, 0.f), 36.f, (float)theView->getWindowWidth() * 0.01f, (float)theView->getWindowHeight() * 0.95f);

	// PLAYER HUD
	auto gameC = testEntity->getComponent<GameplayComponent>();
	if (gameC)
	{
		std::ostringstream ss1;
		std::string collectionStatus;
		ss1 << "KEY: ";
		if (gameC->getHasKey())
		{
			collectionStatus = "COLLECTED";
		}
		theView->RenderTextOnScreen(m_meshList[TEXT_FONT], ss1.str() + collectionStatus, Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.01f, (float)theView->getWindowHeight() * 0.f);

		if (gameC->getKeyShowTimer() > 0)
		{
			theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "KEY REQUIRED", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.75f, (float)theView->getWindowHeight() * 0.f);
		}

		// GAME SAVED
		else if (gameSaved)
		{
			theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "GAME SAVING..", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.75f, (float)theView->getWindowHeight() * 0.f);
		}
	}

	// GAME PAUSE
	if (state == GAMESTATE::STATE_PAUSE)
	{
		theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "GAME PAUSED", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.75f, (float)theView->getWindowHeight() * 0.f);
	}
}

void StateTest::RenderAI()
{
	theView->modelStack.PushMatrix();
	theView->modelStack.Translate(this->testGuard->getComponent<InformationComponent>()->getPosition().x, this->testGuard->getComponent<InformationComponent>()->getPosition().y, this->testGuard->getComponent<InformationComponent>()->getPosition().z);
	theView->modelStack.Rotate(this->testGuard->getComponent<InformationComponent>()->getRotation().y, 0.f, 1.f, 0.f);
	theView->RenderMesh(this->testGuard->getComponent<GraphicsComponent>()->getMesh(this->testGuard->getComponent<GraphicsComponent>()->getResLevel()), false, false);
	theView->modelStack.PopMatrix();
}

void StateTest::Draw(StateHandler * stateHandler)
{
	if (testMap)
	{
		theView->modelStack.PushMatrix();
		theView->modelStack.Translate(0.f, -(float)testMap->getTileSize(), 0.f);
		//testMap->RenderGrids(theView, m_meshList[0], true);
		testMap->RenderLevel(theView);
		testMap->RenderGridEntities(theView);
		RenderPlayer();
		RenderGUI();
		RenderAI();
		theView->modelStack.PopMatrix();
	}

	if (!m_meshList.empty())
	{
		for (int i = 0; i < m_meshList.size(); i++)
		{
			if (i != 0)
			{
				theView->RenderMesh(m_meshList[i], false, false);
			}
		}
	}
	theView->SwapBuffers();
}

void StateTest::gameSave(InformationComponent *infoC)
{
	// OPEN SCRIPT
	LuaReader script("Scripts//SavePlayer.lua");

	// SAVE PLAYER DATA
	float indexX = infoC->getPosition().x / (testMap->getMapWidth() * testMap->getTileSize()) * testMap->getMapWidth();
	float indexY = infoC->getPosition().y / (testMap->getMapHeight() * testMap->getTileSize()) * testMap->getMapHeight();
	int playerIndexX = (int)indexX;
	int playerIndexY = testMap->getMapHeight() - (int)indexY;
	auto gameC = testEntity->getComponent<GameplayComponent>();
	std::vector<int> entityBoxesX; 
	std::vector<int> entityBoxesY;
	std::vector<int> entityDoorsX;
	std::vector<int> entityDoorsY;
	
	// SAVE MAP DATA
	for (unsigned int i = 0; i < testMap->getGridMap().size(); i++)
	{
		for (unsigned int j = 0; j < testMap->getGridMap()[i].size(); j++)
		{
			if (testMap->getGridMap()[i][j]->getGridEntity() != NULL)
			{
				EntityGridObject::OBJECT_TYPE objType = EntityGridObject::OBJECT_TYPE(testMap->getGridMap()[i][j]->getGridEntityType());
				switch (objType)
				{
					case EntityGridObject::OBJECT_BOX:
						entityBoxesX.push_back(i);
						entityBoxesY.push_back(j);
					break;
				}
			}

			if (testMap->getGridMap()[i][j]->getremoveDoor())
			{
				entityDoorsX.push_back(i);
				entityDoorsY.push_back(j);
			}
		}
	}

	// SAVING DATA INTO SCRIPTS
	if (gameC)
	{
		script.savePlayer(playerIndexX, playerIndexY, gameC->getCurrLevel(), gameC->getHasKey());
		script.saveMap(entityBoxesX, entityBoxesY, entityDoorsX, entityDoorsY);
		entityBoxesX.clear();
		entityBoxesY.clear();
		entityDoorsX.clear();
		entityDoorsY.clear();
	}
}

void StateTest::loadPlayer(GridMap *testMap, InformationComponent *informationComponent, GameplayComponent *gameC)
{
	LuaReader Script("Scripts//SavePlayer.lua");
	int x = Script.get<int>("SavePlayer.playerGridX");
	int y = Script.get<int>("SavePlayer.playerGridY");
	int savedLevel = Script.get<int>("SavePlayer.level");
	int collected = Script.get<int>("SavePlayer.hasKey");
	
	if (gameC)
	{
		informationComponent->setPosition(testMap->getGridMap()[y][x]->getGridPos());
		gameC->setCurrLevel(savedLevel);
		if (collected == 1)
		{
			gameC->setHasKey(true);
		}
		
		else
		{
			gameC->setHasKey(false);
		}
	}
}

void StateTest::loadLevel1(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, int gameType)
{
	testMap->LoadData("MapData//level1_Background.csv");
	
	switch (gameType)
	{
		// NEW GAME
		case 1:
			for (int i = 0; i < NUM_BOXES; i++)
			{
				testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
				graphicsComponent = new GraphicsComponent();
				graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
				graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
				testGridObject->addComponent(graphicsComponent);
				testMap->getGridMap()[17][2 + i]->addGridEntity(testGridObject);
			}

			for (int i = 0; i < NUM_SWITCHES; i++)
			{
				testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
				testGridObject->addChildren(19, 4, EntityGridObject::OBJECT_DOOR, testMap);
				graphicsComponent = new GraphicsComponent();
				graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
				testGridObject->addComponent(graphicsComponent);
				testMap->getGridMap()[23][6]->addGridEntity(testGridObject);
			}
		break;

		// LOAD GAME
		case 2:
			for (int i = 0; i < NUM_BOXES; i++)
			{
				LuaReader Script2("Scripts//SaveMap.lua");
				int x = Script2.get<int>("SaveMap.entityX" + std::to_string(i + 1));
				int y = Script2.get<int>("SaveMap.entityY" + std::to_string(i + 1));
				testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
				graphicsComponent = new GraphicsComponent();
				graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
				graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
				testGridObject->addComponent(graphicsComponent);
				testMap->getGridMap()[y][x]->addGridEntity(testGridObject);
			}

			for (int i = 0; i < NUM_SWITCHES; i++)
			{
				LuaReader Script2("Scripts//SaveMap.lua");
				int x = Script2.get<int>("SaveMap.entityDoorX" + std::to_string(i + 1));
				int y = Script2.get<int>("SaveMap.entityDoorY" + std::to_string(i + 1));
				testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
				testGridObject->addChildren(y, x, EntityGridObject::OBJECT_UNDEFINED, testMap);
				graphicsComponent = new GraphicsComponent();
				graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
				testGridObject->addComponent(graphicsComponent);
				testMap->getGridMap()[23][6]->addGridEntity(testGridObject);
			}
		break;
	}

	// KEY
	if (gameC)
	{
		if (!gameC->getHasKey())
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_KEY);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("key", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_key.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[23][2]->addGridEntity(testGridObject);
		}
	}
}

void StateTest::loadLevel2(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, int gameType)
{
	testMap->LoadData("MapData//level2_Background.csv");
}

void StateTest::loadLevel3(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, int gameType)
{
}