#include "StateAGDevMenu.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateAGDevInstructions.h"
#include "StateAGDevHighscore.h"
#include "StateAGDevOptions.h"
#include "StateCredits.h"
#include "StateMenu.h"
#include "SoundManager.h"

bool StateAGDevMenu::runOnce = false;

StateAGDevMenu::~StateAGDevMenu()
{

}

State * StateAGDevMenu::getInstance()
{
	return this;
}

void StateAGDevMenu::Init()
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
	newMesh->textureArray[0] = LoadTGA("Images//GameMenubg.tga");
	//newMesh->alpha = 0.f;
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Project", Color(0.f, 0.f, 0.f), 1.f);
	newMesh->textureArray[0] = LoadTGA("Images//Project.tga");
	//newMesh->alpha = 0.f;
	m_meshList.push_back(newMesh);

	// Create Gui Buttons
	Gui * newGui;
	newGui = new GuiButton("Start Button", "Start", 0.5f, 0.5f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("StartBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Instructions Button", "Instructions", 0.5f, 0.6f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("InstructionsBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Highscores Button", "Highscores", 0.5f, 0.7f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("HighscoreBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Options Button", "Options", 0.5f, 0.8f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("OptionsBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Credits Button", "Credits", 0.5f, 0.9f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("CreditsBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	m_bStartFadeIn = true;
	m_bStartFadeOut = false;
	m_dFadeDelay = 0.0;
}

void StateAGDevMenu::Update(StateHandler * stateHandler, double dt)
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

void StateAGDevMenu::HandleEvents(StateHandler * stateHandler)
{

}

void StateAGDevMenu::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateAGDevMenu::Cleanup()
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

void StateAGDevMenu::Pause()
{

}

void StateAGDevMenu::Resume()
{

}

void StateAGDevMenu::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderButtons();
	theView->SwapBuffers();
}

void StateAGDevMenu::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	theView->Render2DMesh(m_meshList[2], false, false, 400.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), 150.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.7f);
}

void StateAGDevMenu::RenderButtons()
{
	for (unsigned int i = 0; i < m_guiList.size(); i++)
	{
		theView->RenderGui(m_guiList[i], m_meshList[TEXT_FONT]);
	}
}

void StateAGDevMenu::UpdateSelection(StateHandler * stateHandler)
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
						case START_BUTTON:
							SoundManager::playSound("Sounds//select.ogg", false);
							stateHandler->ChangeState(new StateMenu("AGDev GameMenu State", theView));
							break;
						case INSTRUCTIONS_BUTTON:
							SoundManager::playSound("Sounds//select.ogg", false);
							stateHandler->ChangeState(new StateAGDevInstructions("AGDev Instructions State", theView));
							break;
						case HIGHSCORE_BUTTON:
							SoundManager::playSound("Sounds//select.ogg", false);
							stateHandler->ChangeState(new StateAGDevHighscore("AGDev Highscore State", theView));
							break;
						case OPTIONS_BUTTON:
							SoundManager::playSound("Sounds//select.ogg", false);
							stateHandler->ChangeState(new StateAGDevOptions("AGDev Options State", theView));
							break;
						case CREDITS_BUTTON:
							SoundManager::playSound("Sounds//select.ogg", false);
							stateHandler->ChangeState(new StateCredits("AGDev Credits State", theView));
							break;
						case EXIT_BUTTON:
							//Theres no Exit button
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

void StateAGDevMenu::FadeInEffect(double dt)
{
	if (m_meshList[0]->alpha < 1)
	{
		for (Mesh * mesh : m_meshList)
		{
			if (alpha && !runOnce)
			{
				runOnce = true;
				m_meshList[1]->alpha = 0.f;
				m_meshList[2]->alpha = 0.f;
			}
			if (!alpha && runOnce)
			{
				m_meshList[1]->alpha = 1.f;
			}
			mesh->alpha += 2.f * dt;
		}
	}
	else
	{
		m_bStartFadeIn = false;
	}
}

void StateAGDevMenu::FadeOutEffect(double dt, StateHandler * stateHandler)
{
	for (Mesh * mesh : m_meshList)
	{
		mesh->alpha -= 2.f * dt;
	}

	/*if (m_meshList[0]->alpha < 0)
	{
		stateHandler->ChangeState(new StateTest("Test Game State", theView));
	}*/
}