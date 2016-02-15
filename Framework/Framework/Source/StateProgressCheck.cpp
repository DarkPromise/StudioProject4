#include "StateProgressCheck.h"
#include "View.h"
#include "LoadTGA.h"

StateProgressCheck::~StateProgressCheck()
{

}

State * StateProgressCheck::getInstance()
{
	return this;
}

void StateProgressCheck::Init()
{
	// AI Assignment is 2D
	theView->LoadOrthoCamera();

	Mesh * testMesh;
	testMesh = MeshBuilder::GenerateAxes("Axes", 100.f, 100.f, 100.f);
	m_meshList.push_back(testMesh);

	testMesh =MeshBuilder::GenerateText("Source Font", 16, 16);
	testMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(testMesh);

	testMesh = MeshBuilder::GenerateQuad("Background", Color(1.f, 1.f, 1.f), 1.f);
	m_meshList.push_back(testMesh);
	
	testMesh = MeshBuilder::GenerateQuad("Wall", Color(1.f, 1.f, 1.f), 1.f);
	m_meshList.push_back(testMesh);

	testMesh = MeshBuilder::GenerateQuad("Random Enemy 1", Color(1.f, 0.f, 0.f), 1.f);
	m_meshList.push_back(testMesh);

	testMesh = MeshBuilder::GenerateQuad("Random Enemy 2", Color(0.f, 1.f, 0.f), 1.f);
	m_meshList.push_back(testMesh);

	testMesh = MeshBuilder::GenerateQuad("Random Enemy 3", Color(0.f, 0.f, 1.f), 1.f);
	m_meshList.push_back(testMesh);

	testMesh = MeshBuilder::GenerateQuad("Base", Color(1.f, 1.f, 1.f), 1.f);
	m_meshList.push_back(testMesh);
}

void StateProgressCheck::Update(StateHandler * stateHandler, double dt)
{
	theView->Update(dt);
}

void StateProgressCheck::HandleEvents(StateHandler * stateHandler)
{

}

void StateProgressCheck::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateProgressCheck::Cleanup()
{
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

void StateProgressCheck::Pause()
{

}

void StateProgressCheck::Resume()
{

}

void StateProgressCheck::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderWall();
	RenderEnemies();
	RenderBase();
	RenderMessageBoard();
	theView->SwapBuffers();
}

void StateProgressCheck::RenderBackground()
{
	//theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
}

void StateProgressCheck::RenderWall()
{
	theView->Render2DMesh(m_meshList[OBJECT_WALL], false, false, (float)theView->getWindowWidth() * 0.01f, (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
}

void StateProgressCheck::RenderEnemies()
{
	theView->Render2DMesh(m_meshList[OBJECT_ENEMY1], false, false, 100.f, 100.f, (float)theView->getWindowWidth() * 0.75f, (float)theView->getWindowHeight() * 0.85f);
	theView->Render2DMesh(m_meshList[OBJECT_ENEMY2], false, false, 100.f, 100.f, (float)theView->getWindowWidth() * 0.6f, (float)theView->getWindowHeight() * 0.5f);
	theView->Render2DMesh(m_meshList[OBJECT_ENEMY3], false, false, 100.f, 100.f, (float)theView->getWindowWidth() * 0.75f, (float)theView->getWindowHeight() * 0.15f);
}

void StateProgressCheck::RenderBase()
{
	theView->Render2DMesh(m_meshList[OBJECT_BASE], false, false, 100.f, 150.f, (float)theView->getWindowWidth() * 0.f, (float)theView->getWindowHeight() * 0.5f);
}

void StateProgressCheck::RenderMessageBoard()
{
	theView->RenderTextOnScreen(m_meshList[OBJECT_TEXT], "Testing Message Board : Idle", Color(1.f, 1.f, 1.f), 24.f, 0.f, 0.f);
}