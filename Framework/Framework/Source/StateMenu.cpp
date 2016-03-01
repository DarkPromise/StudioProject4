#include "StateMenu.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateTest.h"
#include "StateAGDevMenu.h"
#include "StateLevelSelect.h"
#include "SoundManager.h"

StateMenu::~StateMenu()
{

}

State * StateMenu::getInstance()
{
	return this;
}

void StateMenu::Init()
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

	// Create Gui Buttons
	Gui * newGui;
	newGui = new GuiButton("New Game Button", "New Game", 0.5f, 0.55f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("NewGameBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Load Game Button", "Load Game", 0.5f, 0.65f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("LoadGameBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Level Select Button", "Level Select", 0.5f, 0.75f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("LoadGameBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	m_bStartFadeIn = true;
	m_bStartFadeOut = false;
	m_dFadeDelay = 0.0;
}

void StateMenu::Update(StateHandler * stateHandler, double dt)
{
	if (m_bStartFadeIn)
	{
		FadeInEffect(dt);
	}

	if (m_bStartFadeOut)
	{
		FadeOutEffect(dt, stateHandler);
	}

	UpdateSelection(stateHandler);
	theView->Update(dt);
}

void StateMenu::HandleEvents(StateHandler * stateHandler)
{
	if ((theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE)))
	{
		if (theView->getInputHandler()->getPressDelay() > PRESS_DELAY)
		{
			SoundManager::playSound("Sounds//return.ogg", false);
			stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, false));
			theView->getInputHandler()->setPressDelay(0.0);
		}
	}
}

void StateMenu::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateMenu::Cleanup()
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

void StateMenu::Pause()
{

}

void StateMenu::Resume()
{

}

void StateMenu::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderButtons();
	theView->SwapBuffers();
}

void StateMenu::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	theView->Render2DMesh(m_meshList[2], false, false, 400.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), 150.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.7f);
}

void StateMenu::RenderButtons()
{
	for (unsigned int i = 0; i < m_guiList.size(); i++)
	{
		theView->RenderGui(m_guiList[i], m_meshList[TEXT_FONT]);
	}
}

void StateMenu::UpdateSelection(StateHandler * stateHandler)
{
	if (!m_bStartFadeIn)
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
							case NEWGAME_BUTTON:
								gameType = 1;
								m_bStartFadeOut = true;
								SoundManager::playSound("Sounds//select.ogg", false);
							break;

							case LOADGAME_BUTTON:
								gameType = 2;
								m_bStartFadeOut = true;
								SoundManager::playSound("Sounds//select.ogg", false);
							break;

							case LEVELSELECT_BUTTON:
								stateHandler->ChangeState(new StateLevelSelect("Level select Menu", theView));
								SoundManager::playSound("Sounds//select.ogg", false);
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

void StateMenu::FadeInEffect(double dt)
{
	if (m_meshList[0]->alpha < 2)
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

void StateMenu::FadeOutEffect(double dt, StateHandler * stateHandler)
{
	for (Mesh * mesh : m_meshList)
	{
		if (m_meshList[0] && !runOnce)
		{
			runOnce = true;
			m_meshList[1]->alpha = 2.f;
			m_meshList[2]->alpha = 2.f;
		}
		mesh->alpha -= 2.f * dt;
	}

	if (m_meshList[0]->alpha < 0)
	{
		stateHandler->ChangeState(new StateTest("Test Game State", theView, gameType, 0));
	}
}