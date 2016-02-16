#include "StateSplash.h"
#include "View.h"
#include "StateAGDevMenu.h"
#include "StateTest.h"

StateSplash::~StateSplash()
{

}

State * StateSplash::getInstance()
{
	return this;
}

void StateSplash::Init()
{
	// Splash Screen is 2D
	theView->LoadOrthoCamera();

	// Create Meshes
	Mesh * newMesh;
	newMesh = MeshBuilder::GenerateQuad("Splash BG", Color(1.f, 1.f, 1.f), 1.f);
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Logo", Color(0.f, 0.f, 0.f), 1.f);
	newMesh->textureArray[0] = LoadTGA("Images//Logo.tga");
	m_meshList.push_back(newMesh);

	m_bStartFadeOut = true;
	m_dFadeDelay = 0.0;
}

void StateSplash::Update(StateHandler * stateHandler, double dt)
{
	m_dFadeDelay += dt;

	if (m_dFadeDelay > 1.5)
	{
		if (m_bStartFadeOut)
		{
			FadeOutEffect(dt, stateHandler);
		}
	}

	theView->Update(dt);
}

void StateSplash::HandleEvents(StateHandler * stateHandler)
{

}

void StateSplash::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateSplash::Cleanup()
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

void StateSplash::Pause()
{

}

void StateSplash::Resume()
{

}

void StateSplash::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	theView->Render2DMesh(m_meshList[1], false, false, 512.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), 512.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	theView->SwapBuffers();
}

void StateSplash::RenderBackground()
{
	theView->Render2DMesh(m_meshList[0], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
}

void StateSplash::FadeOutEffect(double dt, StateHandler * stateHandler)
{	
	for (Mesh * mesh : m_meshList)
	{
		mesh->alpha -= 2.f * dt;
	}
	if (m_meshList[0]->alpha < 0)
	{
		stateHandler->ChangeState(new StateAGDevMenu("Menu State", theView));
	}
}