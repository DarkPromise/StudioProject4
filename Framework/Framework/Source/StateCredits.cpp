#include "StateCredits.h"
#include "View.h"
#include "StateAGDevMenu.h"
#include "LoadTGA.h"
#include "StateMainmenu.h"

StateCredits::~StateCredits()
{
}

State * StateCredits::getInstance()
{
	return this;
}

void StateCredits::Init()
{
	// Menu in 2D
	theView->LoadOrthoCamera();

	// Create Meshes
	Mesh * newMesh;
	newMesh = MeshBuilder::GenerateText("Source Font", 16, 16);
	newMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(newMesh);


	newMesh = MeshBuilder::GenerateQuad("Menu BG", Color(1.f, 1.f, 1.f), 1.f);
	m_meshList.push_back(newMesh);

}

void StateCredits::Cleanup()
{
	if (!m_meshList.empty())
	{
		for (int i = 0; i < m_meshList.size(); i++)
		{
			delete m_meshList[i];
		}
	}
	m_meshList.clear();
	m_meshList.~vector();
}

void StateCredits::Pause()
{

}

void StateCredits::Resume()
{

}

void StateCredits::Update(StateHandler * stateHandler, double dt)
{
	theView->Update(dt);
}

void StateCredits::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE))
	{
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView));
	}
}

void StateCredits::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{
}

void StateCredits::Draw(StateHandler* stateHandler)
{
	RenderBackground();
	theView->SwapBuffers();
}

void StateCredits::RenderBackground()
{
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Done by: ", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.4f, (float)theView->getWindowHeight() * 0.5);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Giggs 123132 ", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.4f, (float)theView->getWindowHeight() * 0.4);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Joshua 123132 ", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.4f, (float)theView->getWindowHeight() * 0.3);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Darren 123132 ", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.4f, (float)theView->getWindowHeight() * 0.2);
}