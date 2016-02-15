#include "StateAGDevInstructions.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateAGDevMenu.h"

StateAGDevInstructions::~StateAGDevInstructions()
{

}

State * StateAGDevInstructions::getInstance()
{
	return this;
}

void StateAGDevInstructions::Init()
{
	// Menu in 2D
	theView->LoadOrthoCamera();

	// Enable Mouse
	theView->getInputHandler()->setMouseEnabled(true);

	// Create Meshes
	Mesh * newMesh;
	newMesh = MeshBuilder::GenerateText("Source Font", 16, 16);
	newMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("AGDev Menu BG", Color(1.f, 1.f, 1.f), 1.f);
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Project", Color(0.f, 0.f, 0.f), 1.f);
	newMesh->textureArray[0] = LoadTGA("Images//Project.tga");
	m_meshList.push_back(newMesh);
}

void StateAGDevInstructions::Update(StateHandler * stateHandler, double dt)
{
	theView->Update(dt);
}

void StateAGDevInstructions::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE))
	{
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView));
	}
}

void StateAGDevInstructions::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateAGDevInstructions::Cleanup()
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

void StateAGDevInstructions::Pause()
{

}

void StateAGDevInstructions::Resume()
{

}

void StateAGDevInstructions::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderInstructions();
	theView->SwapBuffers();
}

void StateAGDevInstructions::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	theView->Render2DMesh(m_meshList[2], false, false, 400.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), 150.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.7f);
}

void StateAGDevInstructions::RenderInstructions()
{
	std::string Instructions = "Movement - W A S D Keys";
	std::string Instructions2 = "Avoid the Red Spheres and Pick up the Green Spheres";
	std::string Instructions3 = "Pick up the White Spheres to gain back Health";
	std::string Instructions4 = "Try to get the Highiest Score!";
	std::string Instructions5 = "Press Backspace to go back to the Menu";

	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], Instructions, Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.5f - (Instructions.length() * 10.f), (float)theView->getWindowHeight() * 0.4f);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], Instructions2, Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.5f - (Instructions2.length() * 10.f), (float)theView->getWindowHeight() * 0.3f);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], Instructions3, Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.5f - (Instructions3.length() * 10.f), (float)theView->getWindowHeight() * 0.2f);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], Instructions4, Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.5f - (Instructions4.length() * 10.f), (float)theView->getWindowHeight() * 0.1f);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], Instructions5, Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.5f - (Instructions5.length() * 10.f), (float)theView->getWindowHeight() * 0.f);
}