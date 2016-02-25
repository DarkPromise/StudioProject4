#include "StateTest.h"
#include "View.h"
#include "StateAGDevMenu.h"
#include "StateAGDevOptions.h"
#include "StateGameOver.h"

#define HUD_DISPLAY_DELAY		3
#define xSize					32
#define ySize					25

static int totalBoxes = 0;
static int totalCloseDoors = 0, totalOpenDoors = 0;

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
	testEntity = playerScript.createEntity("Player", theCamera, theView->getInputHandler(), testMap);
	auto informationComponent = testEntity->getComponent<InformationComponent>();
	auto gameplayComponent = testEntity->getComponent<GameplayComponent>();
	auto graphicsComponent = testEntity->getComponent<GraphicsComponent>();
	auto gameC = testEntity->getComponent<GameplayComponent>();

	// Test AI Guard
	/*LuaReader guardScript("Scripts//Guard.lua");
	testGuard = guardScript.createEntity("Guard", theCamera, theView->getInputHandler(), testMap);
	testMap->addGridEntity(testGuard);*/

	switch (gameType)
	{
	case GAMETYPE_NEWGAME:
		resetAllEntityCount();
		informationComponent->setPosition(testMap->getGridMap()[23][1]->getGridPos());
		testMap->addGridEntity(testEntity);
		break;

	case GAMETYPE_LOADGAME:
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

	//highscore.ReadFromTextFile();
}

void StateTest::Update(StateHandler * stateHandler, double dt)
{
	static bool click = false, click2 = false;
	auto controlC = testEntity->getComponent<ControllerComponent>();
	auto infoC = testEntity->getComponent<InformationComponent>();
	auto gameC = testEntity->getComponent<GameplayComponent>();
	auto graphicsComponent = testEntity->getComponent<GraphicsComponent>();

	//std::cout << "Closedoors: " << totalCloseDoors << " Opendoors: " << totalOpenDoors << std::endl;
	//std::cout << "Boxes: " << totalBoxes << std::endl;

	// LEVEL CLEARED
	if (gameC)
	{
		if (gameC->getRestartLevel())
		{
			gameC->setRestartLevel(false);
			this->RestartLevel();
		}

		else if (gameC->getLevelCleared())
		{
			gameC->incrementLevel();
			gameC->setLevelCleared(false);
			gameC->setHasKey(false);
			resetAllEntityCount();
			gameType = GAMETYPE_NEWGAME;

			testMap->removeGridEntity(testEntity);
			testMap->ResetData();
			testMap->Init(xSize, ySize);

			/*LuaReader guardScript("Scripts//Guard.lua");
			testGuard = guardScript.createEntity("Guard", theCamera, theView->getInputHandler(), testMap);
			testMap->addGridEntity(testGuard);*/

			switch (gameC->getCurrLevel())
			{
			case 2:
				if (infoC)
				{
					infoC->setPosition(testMap->getGridMap()[23][30]->getGridPos());
					testMap->addGridEntity(testEntity);
				}
				loadLevel2(testMap, graphicsComponent, testGridObject, gameC, gameType);
				break;

			case 3:
				//loadLevel3(testMap, graphicsComponent, testGridObject, gameC, gameType);
				state = GAMESTATE::STATE_GAMEOVER;
				break;
			}
		}
	}

	// GAME RUNNING
	if (state == GAMESTATE::STATE_PLAY)
	{
		// UPDATE AI
		/*if (testGuard->getComponent<AIComponent>())
		{
			testGuard->getComponent<AIComponent>()->Update(dt, testMap, testEntity);
		}*/

		// PAUSE GAME
		if (!click2 && theView->getInputHandler()->IsKeyPressed(GLFW_KEY_P))
		{
			click2 = true;
			state = GAMESTATE::STATE_PAUSE;
		}

		else if (click2 && !theView->getInputHandler()->IsKeyPressed(GLFW_KEY_P))
		{
			click2 = false;
		}

		// SAVE GAME
		if (!click && theView->getInputHandler()->IsKeyPressed(GLFW_KEY_S))
		{
			click = true;
			gameSaved = true;
			gameSave(infoC);
		}

		else if (click && !theView->getInputHandler()->IsKeyPressed(GLFW_KEY_S))
		{
			click = false;
		}

		// SHOW GAME SAVING
		if (gameSaved)
		{
			gameSavedTimer += dt;
			if (gameSavedTimer >= HUD_DISPLAY_DELAY)
			{
				gameSavedTimer = 0;
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
		theCamera->Update(dt, theView->getInputHandler());
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
				controlC->Update(dt, testMap);
			}
		}

		// UPDATE VIEW
		theView->Update(dt);
		theView->viewStack.LoadIdentity();
		theView->viewStack.LookAt(theCamera->getCameraPos().x, theCamera->getCameraPos().y, theCamera->getCameraPos().z,
			theCamera->getCameraTarget().x, theCamera->getCameraTarget().y, theCamera->getCameraTarget().z,
			theCamera->getCameraUp().x, theCamera->getCameraUp().y, theCamera->getCameraUp().z);

		gameTimer += dt;
		playerRecord.update(dt);
	}

	// GAME PAUSED
	else
	{
		// RESUME GAME
		if (!click2 && theView->getInputHandler()->IsKeyPressed(GLFW_KEY_P))
		{
			click2 = true;
			state = GAMESTATE::STATE_PLAY;
		}

		else if (click2 && !theView->getInputHandler()->IsKeyPressed(GLFW_KEY_P))
		{
			click2 = false;
		}
	}
}

void StateTest::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE))
	{
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, true));
	}

	if (state == GAMESTATE::STATE_GAMEOVER)
	{
		stateHandler->ChangeState(new StateGameOver("AGDev Gameover State", theView));
	}
}

void StateTest::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateTest::Cleanup()
{
	// TestGuard is deleted as a GridEntity from now on
	/*if (testGuard)
	{
		delete testGuard;
	}*/

	/*if (testEntity)
	{
		delete testEntity;
	}*/

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

		theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "LEVEL: " + std::to_string(gameC->getCurrLevel()), Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.825f, (float)theView->getWindowHeight() * 0.93f);
	}

	// GAME PAUSE
	if (state == GAMESTATE::STATE_PAUSE)
	{
		theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "GAME PAUSED", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.78f, (float)theView->getWindowHeight() * 0.f);
	}
}

void StateTest::RenderAI()
{
	if (testGuard)
	{
		if (testGuard->getComponent<InformationComponent>())
		{
			if (testGuard->getComponent<GraphicsComponent>())
			{
				theView->modelStack.PushMatrix();
				theView->modelStack.Translate(this->testGuard->getComponent<InformationComponent>()->getPosition().x, this->testGuard->getComponent<InformationComponent>()->getPosition().y, this->testGuard->getComponent<InformationComponent>()->getPosition().z);
				if (testGuard->getComponent<AIComponent>())
				{
					testGuard->getComponent<AIComponent>()->RenderLineOfSight(theView);
				}
				theView->RenderMesh(this->testGuard->getComponent<GraphicsComponent>()->getMesh(this->testGuard->getComponent<GraphicsComponent>()->getResLevel()), false, false);
				theView->modelStack.PopMatrix();
			}
		}
	}
}

void StateTest::Draw(StateHandler * stateHandler)
{
	if (testMap)
	{
		theView->modelStack.PushMatrix();
		theView->modelStack.Translate(0.f, -(float)testMap->getTileSize(), 0.f);
		testMap->RenderGrids(theView, m_meshList[0], true);
		testMap->RenderLevel(theView);
		testMap->RenderGridEntities(theView);
		//RenderAI();
		RenderPlayer();
		RenderGUI();
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

	// RESET ENTITY DOORS FOR ENTITY COUNT
	totalCloseDoors = 0;
	totalOpenDoors = 0;

	// SAVE PLAYER DATA
	float indexX = infoC->getPosition().x / (testMap->getMapWidth() * testMap->getTileSize()) * testMap->getMapWidth();
	float indexY = infoC->getPosition().y / (testMap->getMapHeight() * testMap->getTileSize()) * testMap->getMapHeight();
	int playerIndexX = (int)indexX;
	int playerIndexY = testMap->getMapHeight() - (int)indexY;
	auto gameC = testEntity->getComponent<GameplayComponent>();
	std::vector<int> entityBoxesX; std::vector<int> entityBoxesY;
	std::vector<int> entityDoorsX; std::vector<int> entityDoorsY;
	std::vector<int> entityDoorsOpenX; std::vector<int> entityDoorsOpenY;
	std::vector<int> entitySwitchesX; std::vector<int> entitySwitchesY;
	//std::cout << "X:" + std::to_string(playerIndexX) << " Y:" + std::to_string(playerIndexY) << std::endl;

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

				case EntityGridObject::OBJECT_DOOR:
					entityDoorsX.push_back(i);
					entityDoorsY.push_back(j);
					totalCloseDoors++;
					for (unsigned int x = 0; x < testMap->getGridMap().size(); x++)
					{
						for (unsigned int y = 0; y < testMap->getGridMap()[x].size(); y++)
						{
							if (testMap->getGridMap()[x][y]->getGridEntity() != NULL)
							{
								if (testMap->getGridMap()[x][y]->getGridEntityType() == EntityGridObject::OBJECT_SWITCH)
								{
									auto gridObject = dynamic_cast<EntityGridObject*>(testMap->getGridMap()[x][y]->getGridEntity());
									if (gridObject)
									{
										for (int k = 0; k < gridObject->getChildren().size(); k++)
										{
											if (i == gridObject->getChildren()[k]->get_m_iIndexY() && j == gridObject->getChildren()[k]->get_m_iIndexX())
											{
												entitySwitchesX.push_back(x);
												entitySwitchesY.push_back(y);
											}
										}
									}
								}
							}
						}
					}
					break;
				}
			}

			// DOOR REMOVED
			if (testMap->getGridMap()[i][j]->getremoveDoor())
			{
				entityDoorsOpenX.push_back(i);
				entityDoorsOpenY.push_back(j);
				totalOpenDoors++;
			}
		}
	}

	// SAVING DATA INTO SCRIPTS
	if (gameC)
	{
		script.savePlayer(playerIndexX, playerIndexY, gameC->getCurrLevel(), gameC->getHasKey(), gameTimer);
		script.saveBoxes(entityBoxesX, entityBoxesY, totalBoxes);
		script.saveDoors(entityDoorsX, entityDoorsY, entityDoorsOpenX, entityDoorsOpenY, totalCloseDoors, totalOpenDoors);
		script.saveSwitches(entitySwitchesX, entitySwitchesY);
		entityBoxesX.clear(); entityBoxesY.clear();
		entityDoorsX.clear(); entityDoorsY.clear();
		entityDoorsOpenX.clear(); entityDoorsOpenY.clear();
		entitySwitchesX.clear(); entitySwitchesY.clear();
	}
}

void StateTest::loadPlayer(GridMap *testMap, InformationComponent *informationComponent, GameplayComponent *gameC)
{
	LuaReader Script("Scripts//SavePlayer.lua");
	int x = Script.get<int>("SavePlayer.playerGridX");
	int y = Script.get<int>("SavePlayer.playerGridY");
	int savedLevel = Script.get<int>("SavePlayer.level");
	int collected = Script.get<int>("SavePlayer.hasKey");
	gameTimer = Script.get<float>("SavePlayer.timing");

	if (gameC)
	{
		informationComponent->setPosition(testMap->getGridMap()[y][x]->getGridPos());
		testMap->addGridEntity(testEntity);
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

void StateTest::loadLevel1(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, GAMETYPE gameType)
{
	testMap->LoadData("MapData//level1_Background.csv");

	switch (gameType)
	{
	case GAMETYPE_NEWGAME:
	{
							 // BOXES
							 for (int i = 1; i < 7; i++)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[15][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 1; i < 7; i++)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[10][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 1; i < 7; i++)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[5][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 11; i < 28; i += 4)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[9][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 12; i < 28; i += 2)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[10][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 13; i < 28; i += 4)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[11][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 11; i < 28; i += 4)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[18][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 12; i < 28; i += 2)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[17][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 13; i < 28; i += 4)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[16][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 // SWITCHES
							 for (int i = 0; i < 1; i++)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
								 testGridObject->addChildren(19, 4 + i, EntityGridObject::OBJECT_DOOR, testMap);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[23][5 + i]->addGridEntity(testGridObject);
							 }
	}
		break;

	case GAMETYPE_LOADGAME:
	{
							  LuaReader Script("Scripts//SaveBoxes.lua");
							  int currentLevelTotalBoxes = Script.get<int>("SaveBoxes.totalBoxes");
							  LuaReader Script2("Scripts//SaveDoors.lua");
							  int currentTotalCloseDoors = Script2.get<int>("SaveDoors.totalCloseDoors");
							  int currentTotalOpenDoors = Script2.get<int>("SaveDoors.totalOpenDoors");
							  LuaReader Script3("Scripts//SaveSwitches.lua");

							  // BOXES
							  for (int i = 0; i < currentLevelTotalBoxes; i++)
							  {
								  int x = Script.get<int>("SaveBoxes.entityX" + std::to_string(i + 1));
								  int y = Script.get<int>("SaveBoxes.entityY" + std::to_string(i + 1));
								  testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								  graphicsComponent = new GraphicsComponent();
								  graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								  graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								  testGridObject->addComponent(graphicsComponent);
								  testMap->getGridMap()[y][x]->addGridEntity(testGridObject);
							  }

							  // SWITCHES
							  for (int i = 0; i < currentTotalCloseDoors; i++)
							  {
								  int x = Script2.get<int>("SaveDoors.entityDoorX" + std::to_string(i + 1));
								  int y = Script2.get<int>("SaveDoors.entityDoorY" + std::to_string(i + 1));
								  int x2 = Script3.get<int>("SaveSwitches.entitySwitchX" + std::to_string(i + 1));
								  int y2 = Script3.get<int>("SaveSwitches.entitySwitchY" + std::to_string(i + 1));
								  testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
								  testGridObject->addChildren(y, x, EntityGridObject::OBJECT_DOOR, testMap);
								  graphicsComponent = new GraphicsComponent();
								  graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
								  graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
								  testGridObject->addComponent(graphicsComponent);
								  testMap->getGridMap()[y2][x2]->addGridEntity(testGridObject);
							  }

							  for (int i = 0; i < currentTotalOpenDoors; i++)
							  {
								  int x = Script2.get<int>("SaveDoors.entityDoorOpenX" + std::to_string(i + 1));
								  int y = Script2.get<int>("SaveDoors.entityDoorOpenY" + std::to_string(i + 1));
								  testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
								  testGridObject->addChildren(y, x, EntityGridObject::OBJECT_UNDEFINED, testMap);
								  graphicsComponent = new GraphicsComponent();
								  graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 32.f));
								  graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile2.tga");
								  testGridObject->addComponent(graphicsComponent);
								  testMap->getGridMap()[0][0]->addGridEntity(testGridObject);
							  }
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

void StateTest::loadLevel2(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, GAMETYPE gameType)
{
	testMap->LoadData("MapData//level2_Background.csv");

	switch (gameType)
	{// NEW GAME

	case GAMETYPE_NEWGAME:
	{
							 // BOXES
							 for (int i = 1; i < 17; i++)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[11][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }
							 for (int i = 12; i < 19; ++i)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][1]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }
							 /*	for (int i = 2; i < 9; ++i)
							 {
							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[15][i]->addGridEntity(testGridObject);
							 totalBoxes++;
							 }*/
							 for (int i = 12; i < 18; ++i)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][4]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 2; i < 4; ++i)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[13][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }
							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[15][2]->addGridEntity(testGridObject);
							 totalBoxes++;

							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[16][3]->addGridEntity(testGridObject);
							 totalBoxes++;

							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[18][2]->addGridEntity(testGridObject);
							 totalBoxes++;

							 for (int i = 12; i < 18; ++i)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][6]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 7; i < 9; ++i)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[13][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }
							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[15][8]->addGridEntity(testGridObject);
							 totalBoxes++;

							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[16][7]->addGridEntity(testGridObject);
							 totalBoxes++;

							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[18][8]->addGridEntity(testGridObject);
							 totalBoxes++;

							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[16][5]->addGridEntity(testGridObject);
							 totalBoxes++;

							 for (int i = 12; i < 19; ++i)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][9]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }
							 for (int i = 1; i < 17; i++)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[19][i]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }
							 for (int i = 10; i < 17; i++)
							 {
								 for (int j = 12; j < 14; ++j)
								 {
									 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
									 graphicsComponent = new GraphicsComponent();
									 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
									 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
									 testGridObject->addComponent(graphicsComponent);
									 testMap->getGridMap()[j][i]->addGridEntity(testGridObject);
									 totalBoxes++;
								 }

							 }
							 for (int i = 10; i < 13; i++)
							 {
								 for (int j = 14; j < 16; ++j)
								 {
									 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
									 graphicsComponent = new GraphicsComponent();
									 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
									 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
									 testGridObject->addComponent(graphicsComponent);
									 testMap->getGridMap()[j][i]->addGridEntity(testGridObject);
									 totalBoxes++;
								 }

							 }

							 for (int i = 14; i < 19; ++i)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][16]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }
							 for (int i = 14; i < 18; i++)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][15]->addGridEntity(testGridObject);
								 totalBoxes++;
							 }

							 for (int i = 4; i < 9; i += 2)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][12]->addGridEntity(testGridObject);
								 totalBoxes++;

							 }


							 for (int i = 3; i < 8; i += 2)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][13]->addGridEntity(testGridObject);
								 totalBoxes++;

							 }

							 for (int i = 4; i < 9; i += 2)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][14]->addGridEntity(testGridObject);
								 totalBoxes++;

							 }
							 for (int i = 3; i < 8; i += 2)
							 {
								 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								 graphicsComponent = new GraphicsComponent();
								 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								 testGridObject->addComponent(graphicsComponent);
								 testMap->getGridMap()[i][15]->addGridEntity(testGridObject);
								 totalBoxes++;

							 }

							 // SWTICHES
							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
							 testGridObject->addChildren(6, 23, EntityGridObject::OBJECT_DOOR, testMap);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[5][1]->addGridEntity(testGridObject);

							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
							 testGridObject->addChildren(6, 25, EntityGridObject::OBJECT_DOOR, testMap);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[18][5]->addGridEntity(testGridObject);

							 testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
							 testGridObject->addChildren(6, 27, EntityGridObject::OBJECT_DOOR, testMap);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
							 graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[17][13]->addGridEntity(testGridObject);

							 /*testGridObject->addChildren(19, 4, EntityGridObject::OBJECT_DOOR, testMap);
							 graphicsComponent = new GraphicsComponent();
							 graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
							 testGridObject->addComponent(graphicsComponent);
							 testMap->getGridMap()[14][5]->addGridEntity(testGridObject);*/
	}
		break;

		// LOAD GAME
	case GAMETYPE_LOADGAME:
	{
							  LuaReader Script("Scripts//SaveBoxes.lua");
							  int currentLevelTotalBoxes = Script.get<int>("SaveBoxes.totalBoxes");
							  LuaReader Script2("Scripts//SaveDoors.lua");
							  int currentTotalCloseDoors = Script2.get<int>("SaveDoors.totalCloseDoors");
							  int currentTotalOpenDoors = Script2.get<int>("SaveDoors.totalOpenDoors");
							  LuaReader Script3("Scripts//SaveSwitches.lua");

							  // BOXES
							  for (int i = 0; i < currentLevelTotalBoxes; i++)
							  {
								  int x = Script.get<int>("SaveBoxes.entityX" + std::to_string(i + 1));
								  int y = Script.get<int>("SaveBoxes.entityY" + std::to_string(i + 1));
								  testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								  graphicsComponent = new GraphicsComponent();
								  graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								  graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								  testGridObject->addComponent(graphicsComponent);
								  testMap->getGridMap()[y][x]->addGridEntity(testGridObject);
							  }

							  // SWITCHES
							  for (int i = 0; i < currentTotalCloseDoors; i++)
							  {
								  int x = Script2.get<int>("SaveDoors.entityDoorX" + std::to_string(i + 1));
								  int y = Script2.get<int>("SaveDoors.entityDoorY" + std::to_string(i + 1));
								  int x2 = Script3.get<int>("SaveSwitches.entitySwitchX" + std::to_string(i + 1));
								  int y2 = Script3.get<int>("SaveSwitches.entitySwitchY" + std::to_string(i + 1));
								  testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
								  testGridObject->addChildren(y, x, EntityGridObject::OBJECT_DOOR, testMap);
								  graphicsComponent = new GraphicsComponent();
								  graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
								  graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
								  testGridObject->addComponent(graphicsComponent);
								  testMap->getGridMap()[y2][x2]->addGridEntity(testGridObject);
							  }

							  for (int i = 0; i < currentTotalOpenDoors; i++)
							  {
								  int x = Script2.get<int>("SaveDoors.entityDoorOpenX" + std::to_string(i + 1));
								  int y = Script2.get<int>("SaveDoors.entityDoorOpenY" + std::to_string(i + 1));
								  testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
								  testGridObject->addChildren(y, x, EntityGridObject::OBJECT_UNDEFINED, testMap);
								  graphicsComponent = new GraphicsComponent();
								  graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 32.f));
								  graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile2.tga");
								  testGridObject->addComponent(graphicsComponent);
								  testMap->getGridMap()[0][0]->addGridEntity(testGridObject);
							  }

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
			testMap->getGridMap()[6][1]->addGridEntity(testGridObject);
		}
	}
}

void StateTest::loadLevel3(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, GAMETYPE gameType)
{
}

void StateTest::resetAllEntityCount()
{
	totalBoxes = 0;
	totalCloseDoors = 0;
	totalOpenDoors = 0;
}

void StateTest::RestartLevel()
{
	testMap->ResetData();
	testMap->Init(xSize, ySize);

	auto infoC = testEntity->getComponent<InformationComponent>();
	if (infoC)
	{
		infoC->setPosition(testMap->getGridMap()[23][1]->getGridPos());
		testMap->addGridEntity(testEntity);
	}

	//LuaReader guardScript("Scripts//Guard.lua");
	//testGuard = guardScript.createEntity("Guard", theCamera, theView->getInputHandler(), testMap);
	//testMap->addGridEntity(testGuard);

	auto gameC = testEntity->getComponent<GameplayComponent>();
	gameC->Reset();
	auto graphicsComponent = testEntity->getComponent<GraphicsComponent>();
	switch (gameC->getCurrLevel())
	{
	case 1:
		loadLevel1(testMap, graphicsComponent, testGridObject, gameC, GAMETYPE_NEWGAME);
		break;
	case 2:
		loadLevel2(testMap, graphicsComponent, testGridObject, gameC, GAMETYPE_NEWGAME);
		break;
	case 3:
		loadLevel3(testMap, graphicsComponent, testGridObject, gameC, GAMETYPE_NEWGAME);
		break;
	}
}