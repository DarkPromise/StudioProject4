#include "StateTest.h"

#include "View.h"

#include "InformationComponent.h"
#include "GraphicsComponent.h"

State * StateTest::getInstance()
{
	return this;
}

void StateTest::Init()
{
	// Testing will be done in Perspective
	theView->LoadPerspectiveCamera(90.0);

	Mesh * testMesh;
	testMesh = MeshBuilder::GenerateText("Source Font", 16, 16);
	testMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(testMesh);

	testMesh = MeshBuilder::GenerateAxes("Axes", 1000.f, 1000.f, 1000.f);
	m_meshList.push_back(testMesh);

	testMesh = MeshBuilder::GenerateCube("Cube", Color(0.f, 0.f, 1.f), 10.f);
	m_meshList.push_back(testMesh);

	theCamera = new Camera();

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
	graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Ground", Color(1.f, 0.f, 0.f), 10.f));
	testEntity->addComponent(graphicsComponent);
}

void StateTest::Update(StateHandler * stateHandler, double dt)
{
	theCamera->Update(dt,theView->getInputHandler());
	auto cameraC = testEntity->getComponent<CameraComponent>();
	if (cameraC)
	{
		cameraC->Update(dt);
	}
	
	m_meshList[1]->alpha += dt;
	if (m_meshList[1]->alpha > 2)
	{
		m_meshList[1]->alpha = 0.f;
	}

	// PLAYER UPDATE
	auto infoC = testEntity->getComponent<InformationComponent>();
	float indexX = infoC->getPosition().x / (testMap->getMapWidth() * testMap->getTileSize()) * testMap->getMapWidth();
	float indexY = infoC->getPosition().y / (testMap->getMapHeight() * testMap->getTileSize()) * testMap->getMapHeight();
	infoC->Update(dt);
	//std::cout << "Player Index [" << (int)indexX << "," << testMap->getMapHeight() - (int)indexY << "]" << std::endl;

	static bool keyRight	= false;
	static bool keyLeft		= false;
	static bool keyUp		= false;
	static bool keyDown		= false;

	if (!keyRight && theView->getInputHandler()->IsKeyPressed(GLFW_KEY_D))
	{
		keyRight = true;
		auto * infoComponent = testEntity->getComponent<InformationComponent>();
		if (infoComponent)
		{
			if ((indexX + 1 < testMap->getMapWidth()))
			{
				if (testMap->getGridMap()[testMap->getMapHeight() - (int)indexY][(int)indexX + 1]->getTileID() != Grid::TILE_WALL)
				{
					infoComponent->setPosition(testMap->getGridMap()[testMap->getMapHeight() - (int)indexY][(int)indexX + 1]->getGridPos());
				}
			}
		}
	}

	else if (keyRight && !theView->getInputHandler()->IsKeyPressed(GLFW_KEY_D))
	{
		keyRight = false;
	}

	if (!keyLeft && theView->getInputHandler()->IsKeyPressed(GLFW_KEY_A))
	{
		keyLeft = true;
		auto * infoComponent = testEntity->getComponent<InformationComponent>();
		if (infoComponent)
		{
			if ((indexX - 1 >= 0))
			{
				if (testMap->getGridMap()[testMap->getMapHeight() - (int)indexY][(int)indexX - 1]->getTileID() != Grid::TILE_WALL)
				{
					infoComponent->setPosition(testMap->getGridMap()[testMap->getMapHeight() - (int)indexY][(int)indexX - 1]->getGridPos());
				}
			}
		}
	}

	else if (keyLeft && !theView->getInputHandler()->IsKeyPressed(GLFW_KEY_A))
	{
		keyLeft = false;
	}

	if (!keyUp && theView->getInputHandler()->IsKeyPressed(GLFW_KEY_W))
	{
		keyUp = true;
		auto * infoComponent = testEntity->getComponent<InformationComponent>();
		if (infoComponent)
		{
			if (((testMap->getMapHeight() - (int)indexY - 1) >= 0))
			{
				if (testMap->getGridMap()[testMap->getMapHeight() - (int)indexY - 1][(int)indexX]->getTileID() != Grid::TILE_WALL)
				{
					infoComponent->setPosition(testMap->getGridMap()[testMap->getMapHeight() - (int)indexY - 1][(int)indexX]->getGridPos());
				}
			}
		}
	}

	else if (keyUp && !theView->getInputHandler()->IsKeyPressed(GLFW_KEY_W))
	{
		keyUp = false;
	}

	if (!keyDown && theView->getInputHandler()->IsKeyPressed(GLFW_KEY_S))
	{
		keyDown = true;
		auto * infoComponent = testEntity->getComponent<InformationComponent>();
		if (infoComponent)
		{
			if (((testMap->getMapHeight() - (int)indexY + 1) < testMap->getMapHeight()))
			{
				if (testMap->getGridMap()[testMap->getMapHeight() - (int)indexY + 1][(int)indexX]->getTileID() != Grid::TILE_WALL)
				{
					infoComponent->setPosition(testMap->getGridMap()[testMap->getMapHeight() - (int)indexY + 1][(int)indexX]->getGridPos());
				}
			}
		}
	}

	else if (keyDown && !theView->getInputHandler()->IsKeyPressed(GLFW_KEY_S))
	{
		keyDown = false;
	}

	theView->Update(dt);
	theView->viewStack.LoadIdentity();
	theView->viewStack.LookAt(
		theCamera->getCameraPos().x, theCamera->getCameraPos().y, theCamera->getCameraPos().z,
		theCamera->getCameraTarget().x, theCamera->getCameraTarget().y, theCamera->getCameraTarget().z,
		theCamera->getCameraUp().x, theCamera->getCameraUp().y, theCamera->getCameraUp().z
		);
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

	auto collisionC = this->testEntity->getComponent<CollisionComponent>();
	if (collisionC)
	{
		theView->modelStack.PushMatrix();
		theView->modelStack.Translate(this->testEntity->getComponent<InformationComponent>()->getPosition().x, this->testEntity->getComponent<InformationComponent>()->getPosition().y, this->testEntity->getComponent<InformationComponent>()->getPosition().z);
		theView->modelStack.Rotate(this->testEntity->getComponent<InformationComponent>()->getRotation().y, 0.f, 1.f, 0.f);
		theView->RenderMesh(collisionC->getMesh(), false, false);
		theView->modelStack.PopMatrix();
	}
}

void StateTest::Draw(StateHandler * stateHandler)
{
	if (testMap)
	{
		theView->modelStack.PushMatrix();
		theView->modelStack.Translate(0.f, -(float)testMap->getTileSize(), 0.f);
		testMap->RenderGrids(theView, m_meshList[0], true);
		testMap->RenderBackground(theView);
		renderPlayer();
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