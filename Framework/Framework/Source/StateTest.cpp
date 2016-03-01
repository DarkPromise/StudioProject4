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

	testMesh = MeshBuilder::GenerateQuad("Background", Color(0.f, 0.f, 0.f), 1.f);
	testMesh->textureArray[0] = LoadTGA("Images//Gamebg.tga");
	m_meshList.push_back(testMesh);

	testMesh = MeshBuilder::GenerateQuad("Fade Out Quad", Color(0.f, 0.f, 0.f), 1.f);
	m_meshList.push_back(testMesh);

	// CAMERA
	theCamera = new Camera();
	theView->getInputHandler()->resetMousePosition(theView);
	theView->getInputHandler()->setMouseEnabled(false);

	// MAP
	testMap = new GridMap();
	testMap->Init(xSize, ySize);

	// PLAYER
	LuaReader playerScript("Scripts//Player.lua");
	testEntity = playerScript.createEntity("Player", theCamera, theView->getInputHandler(), testMap);
	auto informationComponent = testEntity->getComponent<InformationComponent>();
	auto gameplayComponent = testEntity->getComponent<GameplayComponent>();
	auto graphicsComponent = testEntity->getComponent<GraphicsComponent>();
	auto gameC = testEntity->getComponent<GameplayComponent>();

	switch (gameType)
	{
		case GAMETYPE_NEWGAME:
			if (levelSelected != 0)
			{
				gameC->setCurrLevel(levelSelected);
			}
			else
			{
				resetAllEntityCount();
			}
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

		case 4:
			loadLevel4(testMap, graphicsComponent, testGridObject, gameC, gameType);
		break;
	}

	//highscore.ReadFromTextFile();

	// Fade Effect
	m_bStartFadeOut = true;
	m_dFadeDelay = 0.0;
}

void StateTest::Update(StateHandler * stateHandler, double dt)
{
	if (m_bStartFadeOut)
	{
		this->FadeOutEffect(dt);
	}

	static bool click = false, click2 = false, click3 = false;
	auto controlC = testEntity->getComponent<ControllerComponent>();
	auto infoC = testEntity->getComponent<InformationComponent>();
	auto gameC = testEntity->getComponent<GameplayComponent>();
	auto graphicsComponent = testEntity->getComponent<GraphicsComponent>();

	// LEVEL CLEARED
	if (gameC)
	{
		if (gameC->getRestartLevel())
		{
			std::cout << "RESTART" << std::endl;
			gameC->setRestartLevel(false);
			this->RestartLevel();
			return;
		}
		
		else if (gameC->getLevelCleared())
		{
			if (gameC->getCurrLevel() == 4)
			{
				state = STATE_GAMEOVER;
				return;
			}
			
			else
			{
				gameC->incrementLevel();
				gameC->setLevelCleared(false);
				gameC->setHasKey(false);
				resetAllEntityCount();
				gameType = GAMETYPE_NEWGAME;

				testMap->removeGridEntity(testEntity);
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
					
					case 4:
						loadLevel4(testMap, graphicsComponent, testGridObject, gameC, gameType);
					break;
				}
			}
		}
	}

	// GAME RUNNING
	if (state == GAMESTATE::STATE_PLAY)
	{
		// UPDATE AI
		for (int i = 0; i < m_guardList.size(); i++)
		{
			if (m_guardList[i]->getComponent<AIComponent>())
			{
				m_guardList[i]->getComponent<AIComponent>()->Update(dt, testMap, testEntity);
			}
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

		// RESET LEVEL MANUALLY
		if (!click3 && theView->getInputHandler()->IsKeyPressed(GLFW_KEY_R))
		{
			click3 = true;
			RestartLevel();
			return;
		}

		else if (click3 && !theView->getInputHandler()->IsKeyPressed(GLFW_KEY_R))
		{
			click3 = false;
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
		if (graphicsComponent)
		{
			for (int i = 0; i < graphicsComponent->getMeshList().size(); i++)
			{
				graphicsComponent->getMesh(i)->alpha += dt;
				if (graphicsComponent->getMesh(i)->alpha > 2)
				{
					graphicsComponent->getMesh(i)->alpha = 0.f;
				}
			}
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
	if ((theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE)))
	{
		if (theView->getInputHandler()->getPressDelay() > PRESS_DELAY)
		{
			SoundManager::playSound("Sounds//return.ogg", false);
			stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, true));
			theView->getInputHandler()->setPressDelay(0.0);
		}
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
	m_guardList.clear();
	m_guardList.~vector();

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
	auto infoC = testEntity->getComponent<InformationComponent>();
	auto graphicsC = testEntity->getComponent<GraphicsComponent>();
	if (infoC)
	{
		if (graphicsC)
		{

				theView->modelStack.PushMatrix();
				theView->modelStack.Translate(infoC->getPosition().x, infoC->getPosition().y, infoC->getPosition().z);
				switch ((int)infoC->getRotation().y)
				{
				case 0:
					theView->RenderMesh(graphicsC->getMesh(0), false, false);
					break;
				case 180:
					theView->RenderMesh(graphicsC->getMesh(1), false, false);
					break;
				}
				theView->modelStack.PopMatrix();
		}
	}
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
	for (int i = 0; i < m_guardList.size(); i++)
	{
		if (m_guardList[i]->getComponent<InformationComponent>())
		{
			if (m_guardList[i]->getComponent<AIComponent>())
			{
				theView->modelStack.PushMatrix();
				theView->modelStack.Translate(m_guardList[i]->getComponent<InformationComponent>()->getPosition().x, m_guardList[i]->getComponent<InformationComponent>()->getPosition().y, m_guardList[i]->getComponent<InformationComponent>()->getPosition().z);
				m_guardList[i]->getComponent<AIComponent>()->RenderLineOfSight(theView);
				theView->modelStack.PopMatrix();
			}
		}
	}
}

void StateTest::RenderBackground()
{
	theView->modelStack.PushMatrix();
	theView->modelStack.Translate(testEntity->getComponent<InformationComponent>()->getPosition().x, testEntity->getComponent<InformationComponent>()->getPosition().y, testEntity->getComponent<InformationComponent>()->getPosition().z);
	theView->modelStack.Scale(theView->getWindowWidth(), theView->getWindowHeight(), 1.f);
	theView->RenderMesh(m_meshList[1], false, false);
	theView->modelStack.PopMatrix();
}

void StateTest::Draw(StateHandler * stateHandler)
{
	this->RenderBackground();
	if (testMap)
	{
		theView->modelStack.PushMatrix();
		theView->modelStack.Translate(0.f, -(float)testMap->getTileSize(), 0.f);
		testMap->RenderGrids(theView, m_meshList[0], true);
		testMap->RenderLevel(theView);
		testMap->RenderGridEntities(theView);
		RenderAI();
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

	this->RenderFadeQuad();

	theView->SwapBuffers();
}

void StateTest::RenderFadeQuad()
{
	theView->Render2DMesh(m_meshList[2], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
}

void StateTest::gameSave(InformationComponent *infoC)
{
	// OPEN SCRIPT
	LuaReader script("Scripts//SavePlayer.lua");

	// RESET ENTITY DOORS COUNT EVERYTIME GAME SAVES
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
	std::vector<int> entitySwitchesUseX; std::vector<int> entitySwitchesUseY;
	std::vector<int> entityWaypointX; std::vector<int> entityWaypointY;
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
										if (!gridObject->getChildren().empty())
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
					}
					break;

				case EntityGridObject::OBJECT_SWITCH:
					auto gridObject = dynamic_cast<EntityGridObject*>(testMap->getGridMap()[i][j]->getGridEntity());
					if (gridObject)
					{
						//std::cout << "SIZE: " + std::to_string(gridObject->getChildren().size()) << std::endl;
						for (int k = 0; k < gridObject->getChildren().size(); k++)
						{
							if (gridObject->getChildren()[k]->getObjectType() != EntityGridObject::OBJECT_DOOR)
							{
								entitySwitchesUseX.push_back(i);
								entitySwitchesUseY.push_back(j);
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

	// SAVE ENEMIES DATA
	for (int i = 0; i < m_guardList.size(); i++)
	{
		if (m_guardList[i]->getComponent<InformationComponent>())
		{
			if (m_guardList[i]->getComponent<AIComponent>())
			{
				if (gameC)
				{
					auto wayC = m_guardList[i]->getComponent<WaypointComponent>();
					if (wayC)
					{
						float indexX2 = m_guardList[i]->getComponent<InformationComponent>()->getPosition().x / (testMap->getMapWidth() * testMap->getTileSize()) * testMap->getMapWidth();
						float indexY2 = m_guardList[i]->getComponent<InformationComponent>()->getPosition().y / (testMap->getMapHeight() * testMap->getTileSize()) * testMap->getMapHeight();
						int aiIndexX = (int)indexX2;
						int aiIndexY = testMap->getMapHeight() - (int)indexY2;
						Vector3 position = Vector3(aiIndexY, aiIndexX, 0);
						Vector3 direction = m_guardList[i]->getComponent<InformationComponent>()->getDirection();
						Vector3 rotation = m_guardList[i]->getComponent<InformationComponent>()->getRotation();
						std::string state = std::to_string(m_guardList[i]->getComponent<AIComponent>()->getState());
						if (state == "0")
						{
							state = "Idle";
						}
						else if (state == "1")
						{
							state = "Patrol";
						}
						else if (state == "2")
						{
							state = "Chase";
						}
						else if (state == "3")
						{
							state = "Pathing";
						}
						int aiSightLength = m_guardList[i]->getComponent<AIComponent>()->getSightLength();
						for (int k = 0; k < wayC->getWaypoints().size(); k++)
						{
							entityWaypointX.push_back(wayC->getWaypoints()[k]->indexX);
							entityWaypointY.push_back(wayC->getWaypoints()[k]->indexY);
						}
						script.saveEnemies(position, direction, rotation, state, aiSightLength, wayC->getWaypoints().size(), entityWaypointX, entityWaypointY, i, gameC->getCurrLevel(), wayC->getNextWaypointIndex());
						entityWaypointX.clear(); entityWaypointY.clear();
					}
				}
			}
		}
	}

	// SAVING DATA INTO SCRIPTS
	if (gameC)
	{
		script.savePlayer(playerIndexX, playerIndexY, gameC->getCurrLevel(), gameC->getHasKey(), gameTimer);
		script.saveBoxes(entityBoxesX, entityBoxesY, totalBoxes);
		script.saveDoors(entityDoorsX, entityDoorsY, entityDoorsOpenX, entityDoorsOpenY, totalCloseDoors, totalOpenDoors);
		script.saveSwitches(entitySwitchesX, entitySwitchesY, entitySwitchesUseX, entitySwitchesUseY);
		entityBoxesX.clear(); entityBoxesY.clear();
		entityDoorsX.clear(); entityDoorsY.clear();
		entityDoorsOpenX.clear(); entityDoorsOpenY.clear();
		entitySwitchesX.clear(); entitySwitchesY.clear();
		entitySwitchesUseX.clear(); entitySwitchesUseY.clear();
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
		testEntity->getComponent<InformationComponent>()->setPosition(testMap->getGridMap()[23][1]->getGridPos());
		testMap->addGridEntity(testEntity);

		// BOXES
		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[17][4]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 3; i < 6; i += 2)
		{

			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[5][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 2; i < 7; i += 2)
		{

			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[6][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 1; i < 7; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[7][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 2; i < 7; i += 2)
		{

			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[8][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 1; i < 7; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[9][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[4][1]->addGridEntity(testGridObject);
		totalBoxes++;

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

		for (int i = 19; i < 21; i++)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[i][10]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 22; i < 24; i++)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[i][10]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 20; i < 23; i++)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[i][11]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[19][12]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[23][12]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 20; i < 23; i++)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[i][13]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		// SWITCHES
		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(19, 4, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[23][6]->addGridEntity(testGridObject);
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
			int x2 = Script3.get<int>("SaveSwitches.entitySwitchUseX" + std::to_string(i + 1));
			int y2 = Script3.get<int>("SaveSwitches.entitySwitchUseY" + std::to_string(i + 1));
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
			testGridObject->addChildren(y, x, EntityGridObject::OBJECT_UNDEFINED, testMap);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch_activate.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[y2][x2]->addGridEntity(testGridObject);
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
	{
	case GAMETYPE_NEWGAME:
	{
		testEntity->getComponent<InformationComponent>()->setPosition(testMap->getGridMap()[23][30]->getGridPos());
		testMap->addGridEntity(testEntity);

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
			int x2 = Script3.get<int>("SaveSwitches.entitySwitchUseX" + std::to_string(i + 1));
			int y2 = Script3.get<int>("SaveSwitches.entitySwitchUseY" + std::to_string(i + 1));
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
			testGridObject->addChildren(y, x, EntityGridObject::OBJECT_UNDEFINED, testMap);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch_activate.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[y2][x2]->addGridEntity(testGridObject);
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
	testMap->LoadData("MapData//level3_Background.csv");
	Entity * guardEntity;

	switch (gameType)
	{
	case GAMETYPE_NEWGAME:
	{
		testEntity->getComponent<InformationComponent>()->setPosition(testMap->getGridMap()[12][29]->getGridPos());
		testMap->addGridEntity(testEntity);

		// AI
		LuaReader guardScript("Scripts//Guards//Guard8.lua");
		guardEntity = guardScript.createEntity("Guard", theCamera, theView->getInputHandler(), testMap);
		m_guardList.push_back(guardEntity);
		testMap->addGridEntity(guardEntity);

		// BOXES
		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[7][25]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 3; i < 7; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[i][27]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 3; i < 6; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[i][28]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 3; i < 8; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[i][29]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 26; i < 30; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[16][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 27; i < 30; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[17][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 27; i < 30; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[18][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 26; i < 29; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[19][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 27; i < 30; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[20][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 27; i < 30; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[21][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[22][28]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 27; i < 30; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[21][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 1; i < 6; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[2][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 2; i < 5; i += 2)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[3][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 2; i < 4; i++)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[4][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[4][5]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[5][2]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[5][5]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 3; i < 5; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[6][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 1; i < 3; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[7][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[7][5]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[8][1]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[8][4]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[17][4]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[18][3]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[19][2]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[19][4]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 2; i < 4; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[20][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 1; i < 6; ++i)
		{
			if (i != 3)
			{
				testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
				graphicsComponent = new GraphicsComponent();
				graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
				graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
				testGridObject->addComponent(graphicsComponent);
				testMap->getGridMap()[21][i]->addGridEntity(testGridObject);
				totalBoxes++;
			}
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[22][4]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 1; i < 5; ++i)
		{
			if (i != 2 && i != 3)
			{
				testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
				graphicsComponent = new GraphicsComponent();
				graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
				graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
				testGridObject->addComponent(graphicsComponent);
				testMap->getGridMap()[23][i]->addGridEntity(testGridObject);
				totalBoxes++;
			}
		}

		// SWTICHES
		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(21, 25, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[7][28]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(18, 28, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[20][30]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(7, 6, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[17][28]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(15, 4, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[6][1]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(13, 14, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[22][3]->addGridEntity(testGridObject);
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

		// AI
		LuaReader guardScript("Scripts//Guards//GuardSave8.lua");
		guardEntity = guardScript.createEntity("Guard", theCamera, theView->getInputHandler(), testMap);
		m_guardList.push_back(guardEntity);
		testMap->addGridEntity(guardEntity);

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
			int x2 = Script3.get<int>("SaveSwitches.entitySwitchUseX" + std::to_string(i + 1));
			int y2 = Script3.get<int>("SaveSwitches.entitySwitchUseY" + std::to_string(i + 1));
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
			testGridObject->addChildren(y, x, EntityGridObject::OBJECT_UNDEFINED, testMap);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch_activate.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[y2][x2]->addGridEntity(testGridObject);
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
			testMap->getGridMap()[12][16]->addGridEntity(testGridObject);
		}
	}
}

void StateTest::loadLevel4(GridMap *testMap, GraphicsComponent *graphicsComponent, EntityGridObject *testGridObject, GameplayComponent *gameC, GAMETYPE gameType)
{
	testMap->LoadData("MapData//level4_Background.csv");
	Entity * guardEntity;

	switch (gameType)
	{
	case GAMETYPE_NEWGAME:
	{
		testEntity->getComponent<InformationComponent>()->setPosition(testMap->getGridMap()[23][16]->getGridPos());
		testMap->addGridEntity(testEntity);

		// AI
		for (int i = 0; i < 7; i++)
		{
			LuaReader guardScript("Scripts//Guards//Guard" + std::to_string(i + 1) + ".lua");
			guardEntity = guardScript.createEntity("Guard", theCamera, theView->getInputHandler(), testMap);
			m_guardList.push_back(guardEntity);
			testMap->addGridEntity(guardEntity);
		}

		// BOXES
		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[1][16]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[1][21]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[1][23]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[1][25]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 17; i < 18; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[2][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[2][23]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[3][16]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[3][21]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[3][23]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 17; i < 18; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[4][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[5][16]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[5][19]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 29; i < 30; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[5][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 28; i < 30; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[6][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 16; i < 18; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[7][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[7][24]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 28; i < 30; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[7][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 29; i < 30; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[9][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 24; i < 25; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[10][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[11][5]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[11][10]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 24; i < 25; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[11][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[12][10]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 24; i < 25; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[12][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[13][10]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[14][10]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[15][10]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 25; i < 27; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[15][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		for (int i = 18; i < 24; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[16][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[18][25]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[18][27]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[19][25]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[19][27]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 20; i < 21; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[20][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[20][25]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[20][28]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[21][16]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[21][20]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[21][29]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 20; i < 21; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[22][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[22][28]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[22][30]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 18; i < 19; ++i)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[23][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[23][29]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[2][18]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[4][18]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[7][18]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[5][28]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 5; i < 8; i++)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[i][30]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[9][30]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[15][27]->addGridEntity(testGridObject);
		totalBoxes++;

		for (int i = 18; i < 20; i++)
		{
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[19][i]->addGridEntity(testGridObject);
			totalBoxes++;
		}

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[23][19]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[20][21]->addGridEntity(testGridObject);
		totalBoxes++;

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[22][21]->addGridEntity(testGridObject);
		totalBoxes++;

		// SWITCHES
		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(5, 26, EntityGridObject::OBJECT_DOOR, testMap);
		testGridObject->addChildren(5, 21, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[1][24]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(4, 27, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[8][17]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(18, 28, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[12][30]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(23, 15, EntityGridObject::OBJECT_DOOR, testMap);
		testGridObject->addChildren(17, 17, EntityGridObject::OBJECT_DOOR, testMap);
		testGridObject->addChildren(14, 15, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[21][18]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(14, 14, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[12][18]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(11, 22, EntityGridObject::OBJECT_DOOR, testMap);
		testGridObject->addChildren(13, 22, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[16][30]->addGridEntity(testGridObject);

		testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
		testGridObject->addChildren(10, 21, EntityGridObject::OBJECT_DOOR, testMap);
		graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch.tga");
		testGridObject->addComponent(graphicsComponent);
		testMap->getGridMap()[8][14]->addGridEntity(testGridObject);
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

		// AI
		for (int i = 0; i < 7; i++)
		{
			LuaReader guardScript("Scripts//Guards//GuardSave" + std::to_string(i + 1) + ".lua");
			guardEntity = guardScript.createEntity("Guard", theCamera, theView->getInputHandler(), testMap);
			m_guardList.push_back(guardEntity);
			testMap->addGridEntity(guardEntity);
		}

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
			int x2 = Script3.get<int>("SaveSwitches.entitySwitchUseX" + std::to_string(i + 1));
			int y2 = Script3.get<int>("SaveSwitches.entitySwitchUseY" + std::to_string(i + 1));
			testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
			testGridObject->addChildren(y, x, EntityGridObject::OBJECT_UNDEFINED, testMap);
			graphicsComponent = new GraphicsComponent();
			graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
			graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch_activate.tga");
			testGridObject->addComponent(graphicsComponent);
			testMap->getGridMap()[y2][x2]->addGridEntity(testGridObject);
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
			testMap->getGridMap()[22][13]->addGridEntity(testGridObject);
		}
	}
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

	m_guardList.clear();

	LuaReader playerScript("Scripts//Player.lua");
	testEntity = playerScript.createEntity("Player", theCamera, theView->getInputHandler(), testMap);
	gameType = GAMETYPE_NEWGAME;

	auto gameC = testEntity->getComponent<GameplayComponent>();
	auto graphicsComponent = testEntity->getComponent<GraphicsComponent>();
	if (gameC)
	{
		gameC->Reset();		
		if (levelSelected != 0)
		{
			gameC->setCurrLevel(levelSelected);
		}
		
		if (graphicsComponent)
		{
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
				
				case 4:
					loadLevel4(testMap, graphicsComponent, testGridObject, gameC, gameType);
				break;
			}
		}
	}
}

void StateTest::FadeInEffect(double dt)
{
	
}

void StateTest::FadeOutEffect(double dt)
{
	float newAlpha = Math::SmoothDamp(m_meshList[2]->alpha, -1.f, 10.f, 0.001f, 10.f, (float)dt);
	m_meshList[2]->alpha = newAlpha;

	if (m_meshList[2]->alpha < 0)
	{
		m_bStartFadeOut = false;
	}
}