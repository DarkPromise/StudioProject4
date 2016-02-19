#include "StateTest.h"
#include "View.h"

State * StateTest::getInstance()
{
	return this;
}

void StateTest::Init()
{
	// Testing will be done in Perspective
	theView->LoadPerspectiveCamera(90.0);

	// OPEN THE LUA SCRIPT
	LuaReader Script("Scripts//Save.lua");
	int x = Script.get<int>("Save.playerGridX");
	int y = Script.get<int>("Save.playerGridY");

	Mesh * testMesh;
	testMesh = MeshBuilder::GenerateText("Source Font", 16, 16);
	testMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(testMesh);

	theCamera = new Camera();
	//SoundManager::playSound("Sounds//movesfx.wav");

	theView->getInputHandler()->resetMousePosition(theView);
	theView->getInputHandler()->setMouseEnabled(false);

	testMap = new GridMap();
	testMap->Init(32, 25, 32);
	testMap->LoadData("MapData//MainMenu_Background.csv","MapData//MainMenu_Foreground.csv");

	// Init Test Entity
	testEntity = new EntityTest();
	
	auto informationComponent = new InformationComponent();
	informationComponent->setName("Test");
	//informationComponent->setPosition(Vector3(0.f, 0.f, 0.f));
	informationComponent->setPosition(testMap->getGridMap()[23][1]->getGridPos());
	testEntity->addComponent(informationComponent);
	
	auto cameraComponent = new CameraComponent(theCamera);
	cameraComponent->setCameraOffset(Vector3(0.f, 0.f, 300.f));
	theCamera->setCameraMode(Camera::CM_THIRD_PERSON_FOLLOW_ENTITY);
	testEntity->addComponent(cameraComponent);

	auto graphicsComponent = new GraphicsComponent();
	graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Player", Color(1.f, 0.f, 0.f), 32.f));
	graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//player.tga");
	graphicsComponent->getMesh()->alpha = 0.7f;
	testEntity->addComponent(graphicsComponent);

	auto controlComponent = new ControllerComponent(theView->getInputHandler());
	testEntity->addComponent(controlComponent);

	// BOXES
	EntityGridObject * testGridObject;
	testGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
	graphicsComponent = new GraphicsComponent();
	graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
	graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
	testGridObject->addComponent(graphicsComponent);
	testMap->getGridMap()[22][2]->addGridEntity(testGridObject);

	// KEY
	testGridObject = new EntityGridObject(EntityGridObject::OBJECT_KEY);
	graphicsComponent = new GraphicsComponent();
	graphicsComponent->addMesh(MeshBuilder::GenerateQuad("key", Color(1.f, 0.f, 0.f), 32.f));
	graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_key.tga");
	testGridObject->addComponent(graphicsComponent);
	testMap->getGridMap()[23][2]->addGridEntity(testGridObject);

	// SWITCHES
	testGridObject = new EntityGridObject(EntityGridObject::OBJECT_SWITCH);
	testGridObject->addChildren(19, 1, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(19, 2, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(19, 3, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(19, 4, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(19, 5, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(19, 6, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(19, 7, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(20, 7, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(21, 7, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(22, 7, EntityGridObject::OBJECT_DOOR, testMap);
	testGridObject->addChildren(23, 7, EntityGridObject::OBJECT_DOOR, testMap);
	graphicsComponent = new GraphicsComponent();
	graphicsComponent->addMesh(MeshBuilder::GenerateQuad("switch", Color(1.f, 0.f, 0.f), 16.f));
	testGridObject->addComponent(graphicsComponent);
	testMap->getGridMap()[23][6]->addGridEntity(testGridObject);
}

void StateTest::Update(StateHandler * stateHandler, double dt)
{
	static bool click = false;
	auto controlC = testEntity->getComponent<ControllerComponent>();
	auto infoC = testEntity->getComponent<InformationComponent>();

	// GAME RUNNING
	if (state == GAMESTATE::STATE_PLAY)
	{
		// PAUSE GAME
		if (!click && controlC->getInputHandler()->IsKeyPressed('P'))
		{
			click = true;
			state = GAMESTATE::STATE_PAUSE;
		}

		else if (click && !controlC->getInputHandler()->IsKeyPressed('P'))
		{
			click = false;
		}

		// SAVE GAME
		if (!click && controlC->getInputHandler()->IsKeyPressed('S'))
		{
			click = true;
			gameSaved = true;

			float indexX = infoC->getPosition().x / (testMap->getMapWidth() * testMap->getTileSize()) * testMap->getMapWidth();
			float indexY = infoC->getPosition().y / (testMap->getMapHeight() * testMap->getTileSize()) * testMap->getMapHeight();
			int playerIndexX = (int)indexX;
			int playerIndexY = testMap->getMapHeight() - (int)indexY;

			LuaReader script("Scripts//Save.lua");
			script.saveFile(playerIndexX, playerIndexY);
		}

		else if (click && !controlC->getInputHandler()->IsKeyPressed('S'))
		{
			click = false;
		}

		// SHOW GAME SAVING
		if (gameSaved)
		{
			gameTimer += dt;
			if (gameTimer >= 5)
			{
				gameTimer = 0;
				gameSaved = false;
			}
		}

		// UPDATE CAMERA
		theCamera->Update(dt,theView->getInputHandler());
		auto cameraC = testEntity->getComponent<CameraComponent>();
		if (cameraC)
		{
			cameraC->Update(dt);
		}
	
		testEntity->getComponent<GraphicsComponent>()->getMesh()->alpha += dt;
		if (testEntity->getComponent<GraphicsComponent>()->getMesh()->alpha > 2)
		{
			testEntity->getComponent<GraphicsComponent>()->getMesh()->alpha = 0.f;
		}

		// PLAYER UPDATE
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
		theView->viewStack.LookAt(
			theCamera->getCameraPos().x, theCamera->getCameraPos().y, theCamera->getCameraPos().z,
			theCamera->getCameraTarget().x, theCamera->getCameraTarget().y, theCamera->getCameraTarget().z,
			theCamera->getCameraUp().x, theCamera->getCameraUp().y, theCamera->getCameraUp().z
			);

		gameTimer += dt;
	}

	// GAME PAUSED
	else
	{
		// RESUME GAME
		if (!click && controlC->getInputHandler()->IsKeyPressed('P'))
		{
			click = true;
			state = GAMESTATE::STATE_PLAY;
		}

		else if (click && !controlC->getInputHandler()->IsKeyPressed('P'))
		{
			click = false;
		}
	}
}

void StateTest::HandleEvents(StateHandler * stateHandler)
{

}

void StateTest::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateTest::Cleanup()
{
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

void StateTest::renderPlayer()
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

void StateTest::renderGUI()
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

	// PLAYER HUD STUFF
	EntityTest *thePlayer = dynamic_cast<EntityTest*>(testEntity);
	if (thePlayer)
	{
		std::ostringstream ss1;
		std::string collectionStatus;
		ss1 << "KEY: ";
		if (thePlayer->m_bHasKey)
		{
			collectionStatus = "COLLECTED";
		}
		theView->RenderTextOnScreen(m_meshList[TEXT_FONT], ss1.str() + collectionStatus, Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.01f, (float)theView->getWindowHeight() * 0.f);
	}

	// GAME PAUSE
	if (state == GAMESTATE::STATE_PAUSE)
	{
		theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "GAME PAUSED", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.75f, (float)theView->getWindowHeight() * 0.f);
	}

	// GAME SAVED
	if (gameSaved)
	{
		theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "GAME SAVING..", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.75f, (float)theView->getWindowHeight() * 0.f);
	}
}

void StateTest::Draw(StateHandler * stateHandler)
{
	if (testMap)
	{
		theView->modelStack.PushMatrix();
		theView->modelStack.Translate(0.f, -(float)testMap->getTileSize(), 0.f);
		//testMap->RenderGrids(theView, m_meshList[0], true);
		testMap->RenderBackground(theView);
		testMap->RenderGridEntities(theView);
		renderPlayer();
		renderGUI();
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