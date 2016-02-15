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
	testMesh = MeshBuilder::GenerateAxes("Axes", 1000.f, 1000.f, 1000.f);
	m_meshList.push_back(testMesh);

	testMesh = MeshBuilder::GenerateCube("Cube", Color(0.f, 0.f, 1.f), 10.f);
	m_meshList.push_back(testMesh);

	theCamera = new Camera();

	theView->getInputHandler()->resetMousePosition(theView);
	theView->getInputHandler()->setMouseEnabled(false);

	testMap = new GridMap();
	testMap->Init(64, 32, 5);
}

void StateTest::Update(StateHandler * stateHandler, double dt)
{
	theCamera->Update(dt,theView->getInputHandler());
	theView->viewStack.LoadIdentity();
	theView->viewStack = theCamera->getViewMatrix();
	theView->Update(dt);

	m_meshList[1]->alpha += dt;
	if (m_meshList[1]->alpha > 2)
	{
		m_meshList[1]->alpha = 0.f;
	}

	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_W))
	{
		Vector3 newPos = theCamera->getCameraPos();
		newPos.z -= 1.f;
		theCamera->setCameraPos(newPos);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_S))
	{
		Vector3 newPos = theCamera->getCameraPos();
		newPos.z += 1.f;
		theCamera->setCameraPos(newPos);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_A))
	{
		Vector3 newPos = theCamera->getCameraPos();
		newPos.x -= 1.f;
		theCamera->setCameraPos(newPos);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_D))
	{
		Vector3 newPos = theCamera->getCameraPos();
		newPos.x += 1.f;
		theCamera->setCameraPos(newPos);
	}

	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_LEFT))
	{
		theCamera->setCameraYaw(theCamera->getCameraYaw() - 1.f);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_RIGHT))
	{
		theCamera->setCameraYaw(theCamera->getCameraYaw() + 1.f);
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

void StateTest::Draw(StateHandler * stateHandler)
{
	if (testMap)
	{
		testMap->RenderGrids(theView, true);
	}

	if (!m_meshList.empty())
	{
		for (int i = 0; i < m_meshList.size(); i++)
		{
			theView->RenderMesh(m_meshList[i],false,false);
		}
	}

	theView->SwapBuffers();
}