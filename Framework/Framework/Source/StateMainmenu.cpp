#include "StateMainmenu.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateAIAssignment.h"
#include "StateMultiplayerMenu.h"
#include "StateSplash.h"

StateMainmenu::~StateMainmenu()
{

}

State * StateMainmenu::getInstance()
{
	return this;
}

void StateMainmenu::Init()
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

	// Create Gui Buttons
	Gui * newGui;
	newGui = new GuiButton("Multiplayer", "Multiplayer Assignment", 0.5f, 0.25f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("MultiplayerBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Artificial Intelliegence", "AI Assignment", 0.5f, 0.5f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("AIBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Advanced Game Dev", "AGDev Assignment", 0.5f, 0.75f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("AGDevBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	// Fade Effect
	m_dFadeDelay = 0.0;
	m_bStartFade = false;
}

void StateMainmenu::Update(StateHandler * stateHandler, double dt)
{
	FadeOutEffect(dt,stateHandler);
	UpdateSelection(stateHandler);
	theView->Update(dt);
}

void StateMainmenu::HandleEvents(StateHandler * stateHandler)
{

}

void StateMainmenu::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateMainmenu::Cleanup()
{
	if (!m_guiList.empty())
	{
		for (int i = 0; i < m_guiList.size(); i++)
		{
			delete m_guiList[i];
		}
	}
	m_guiList.clear();
	m_guiList.~vector();

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

void StateMainmenu::Pause()
{

}

void StateMainmenu::Resume()
{

}

void StateMainmenu::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderButtons();
	theView->SwapBuffers();
}

void StateMainmenu::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
}

void StateMainmenu::RenderButtons()
{
	for (unsigned int i = 0; i < m_guiList.size(); i++)
	{
		theView->RenderGui(m_guiList[i],m_meshList[TEXT_FONT]);
	}
}

void StateMainmenu::UpdateSelection(StateHandler * stateHandler)
{
	if (!m_bStartFade)
	{
		for (unsigned int i = 0; i < m_guiList.size(); i++)
		{
			m_guiList[i]->setX(theView->getWindowWidth() * (m_guiList[i]->getWidthOffset()));
			m_guiList[i]->setY(theView->getWindowHeight() * (m_guiList[i]->getHeightOffset()));

			if (m_guiList[i]->getBoundingBox().Min.x + m_guiList[i]->getX() <= theView->getInputHandler()->getMouseX() &&
				theView->getInputHandler()->getMouseX() <= m_guiList[i]->getBoundingBox().Max.x + m_guiList[i]->getX() &&
				m_guiList[i]->getBoundingBox().Min.y + m_guiList[i]->getY() <= theView->getInputHandler()->getMouseY() &&
				theView->getInputHandler()->getMouseY() <= m_guiList[i]->getBoundingBox().Max.y + m_guiList[i]->getY())
			{
				m_guiList[i]->highlightButton(true);

				if (theView->getInputHandler()->getClickDelay() <= 0.0)
				{
					if (theView->getInputHandler()->IsKeyPressed(GLFW_MOUSE_BUTTON_1))
					{
						switch (MENU_BUTTONS(i))
						{
						case MULTIPLAYER_BUTTON:
							stateHandler->ChangeState(new StateMultiplayerMenu("Multiplayer Menu state", this->theView));
							break;
						case AI_BUTTON:
							stateHandler->ChangeState(new StateAIAssignment("AI State", this->theView));
							break;
						case AGDEV_BUTTON:
							m_bStartFade = true;
							break;
						}
						theView->getInputHandler()->setClickDelay(0.2);
					}
				}
			}
			else
			{
				m_guiList[i]->highlightButton(false);
			}
		}
	}
}

void StateMainmenu::FadeOutEffect(double dt, StateHandler * stateHandler)
{
	if (m_bStartFade)
	{
		m_dFadeDelay += dt;

		// Gui Text Uses the same text mesh
		m_meshList[TEXT_FONT]->alpha -= 2.f * dt;

		for (Mesh * mesh : m_meshList)
		{
			mesh->alpha = m_meshList[TEXT_FONT]->alpha;
		}

		if (m_meshList[TEXT_FONT]->alpha < 0)
		{
			stateHandler->ChangeState(new StateSplash("Splash State", theView));
		}
	}
}