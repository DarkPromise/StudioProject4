#include "StateAGDevHighscore.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateAGDevMenu.h"
#include "SoundManager.h"
#include <iomanip>

StateAGDevHighscore::~StateAGDevHighscore()
{
	highscore.reset();
}

State * StateAGDevHighscore::getInstance()
{
	return this;
}

void StateAGDevHighscore::Init()
{
	// Menu in 2D
	theView->LoadOrthoCamera();

	// Enable Mouse
	theView->getInputHandler()->setMouseEnabled(true);

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

	highscore.ReadFromTextFile();
}

void StateAGDevHighscore::Update(StateHandler * stateHandler, double dt)
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

void StateAGDevHighscore::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE))
	{
		SoundManager::playSound("Sounds//return.ogg", false);
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, false));
	}
}

void StateAGDevHighscore::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateAGDevHighscore::Cleanup()
{
	this->highscore.~Highscore();
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

void StateAGDevHighscore::Pause()
{

}

void StateAGDevHighscore::Resume()
{

}

void StateAGDevHighscore::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderHighscore();
	theView->SwapBuffers();
}

void StateAGDevHighscore::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	theView->Render2DMesh(m_meshList[2], false, false, 400.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), 150.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.7f);
}

void StateAGDevHighscore::RenderHighscore()
{
	for (int i = 0; i < 5; ++i)
	{
		int temp = i;
		std::ostringstream scoring;
		std::stringstream scoring1;

		scoring1 << std::setprecision(2) << highscore.record[i].getTiming().getSec();

		scoring << temp + 1 << "." << highscore.record[i].getName() << " " << highscore.record[i].getTiming().getMin() << ":" << scoring1.str();
		theView->RenderTextOnScreen(m_meshList[TEXT_FONT], scoring.str(), Color(1.f, 0.f, 0.f), 48.f, ((float)theView->getWindowWidth() * 0.5f) - (scoring.str().length() * 10.f), ((float)theView->getWindowHeight() * 0.5f - (i * 70.f)));
	}
}

void StateAGDevHighscore::FadeInEffect(double dt)
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

void StateAGDevHighscore::FadeOutEffect(double dt, StateHandler * stateHandler)
{
	for (Mesh * mesh : m_meshList)
	{
		mesh->alpha -= 2.f * dt;
	}
}