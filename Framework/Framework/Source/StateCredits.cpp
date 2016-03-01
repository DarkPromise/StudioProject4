#include "StateCredits.h"
#include "View.h"
#include "StateAGDevMenu.h"
#include "LoadTGA.h"
#include "SoundManager.h"

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
	newMesh->alpha = 0.f;
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("AGDev Menu BG", Color(1.f, 1.f, 1.f), 1.f);
	//newMesh->alpha = 0.f;
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Project", Color(0.f, 0.f, 0.f), 1.f);
	newMesh->textureArray[0] = LoadTGA("Images//Project.tga");
	//newMesh->alpha = 0.f;
	m_meshList.push_back(newMesh);

	m_bStartFadeIn = true;
	m_bStartFadeOut = false;
	m_dFadeDelay = 0.0;
}

void StateCredits::Update(StateHandler * stateHandler, double dt)
{
	if (m_bStartFadeIn)
	{
		FadeInEffect(dt);
	}

	if (m_bStartFadeOut)
	{
		FadeOutEffect(dt, stateHandler);
	}

	theView->Update(dt);
}

void StateCredits::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE))
	{
		SoundManager::playSound("Sounds//return.ogg", false);
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, false));
	}
}

void StateCredits::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{
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

void StateCredits::Draw(StateHandler* stateHandler)
{
	RenderBackground();
	RenderNames();
	theView->SwapBuffers();
}

void StateCredits::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	theView->Render2DMesh(m_meshList[2], false, false, 400.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), 150.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.7f);
}

void StateCredits::RenderNames()
{
	//theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Done by: ", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.4f, (float)theView->getWindowHeight() * 0.5);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Giggs  140169M", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.37f, (float)theView->getWindowHeight() * 0.4);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Joshua 141404R", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.37f, (float)theView->getWindowHeight() * 0.3);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Darren 141396J", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.37f, (float)theView->getWindowHeight() * 0.2);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Lucid Design (http://luciddesign.tk)", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.2f, (float)theView->getWindowHeight() * 0.1);
}

void StateCredits::FadeInEffect(double dt)
{
	if (m_meshList[0]->alpha < 1)
	{
		for (Mesh * mesh : m_meshList)
		{
			mesh->alpha += 2.f * dt;
		}
	}
	else
	{
		m_bStartFadeIn = false;
	}
}

void StateCredits::FadeOutEffect(double dt, StateHandler * stateHandler)
{
	for (Mesh * mesh : m_meshList)
	{
		mesh->alpha -= 2.f * dt;
	}
}