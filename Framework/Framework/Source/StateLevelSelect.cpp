#include "StateLevelSelect.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateTest.h"
#include "StateMenu.h"
#include "SoundManager.h"

StateLevelSelect::~StateLevelSelect()
{

}

State * StateLevelSelect::getInstance()
{
	return this;
}

void StateLevelSelect::Init()
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
	newGui = new GuiButton("Level1 Button", "Level 1", 0.5f, 0.55f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("Level1", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Level2 Button", "Level 2", 0.5f, 0.65f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("LLevel2", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Level3 Button", "Level 3", 0.5f, 0.75f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("Level3", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Level4 Button", "Level 4", 0.5f, 0.85f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("Level4", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	m_bStartFadeIn = true;
	m_bStartFadeOut = false;
	m_dFadeDelay = 0.0;
}

void StateLevelSelect::Update(StateHandler * stateHandler, double dt)
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

void StateLevelSelect::HandleEvents(StateHandler * stateHandler)
{
	if ((theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE)))
	{
		if (theView->getInputHandler()->getPressDelay() > PRESS_DELAY)
		{
			SoundManager::playSound("Sounds//return.ogg", false);
			stateHandler->ChangeState(new StateMenu("AGDev GameMenu State", theView));
			theView->getInputHandler()->setPressDelay(0.0);
		}
	}
}

void StateLevelSelect::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateLevelSelect::Cleanup()
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

void StateLevelSelect::Pause()
{

}

void StateLevelSelect::Resume()
{

}

void StateLevelSelect::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderButtons();
	theView->SwapBuffers();
}

void StateLevelSelect::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	theView->Render2DMesh(m_meshList[2], false, false, 400.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), 150.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.7f);
}

void StateLevelSelect::RenderButtons()
{
	for (unsigned int i = 0; i < m_guiList.size(); i++)
	{
		theView->RenderGui(m_guiList[i], m_meshList[TEXT_FONT]);
	}
}

void StateLevelSelect::UpdateSelection(StateHandler * stateHandler)
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
							case LEVEL1_BUTTON:
								levelSelected = 1;
								m_bStartFadeOut = true;
								SoundManager::playSound("Sounds//select.ogg", false);
							break;

							case LEVEL2_BUTTON:
								levelSelected = 2;
								m_bStartFadeOut = true;
								SoundManager::playSound("Sounds//select.ogg", false);
							break;

							case LEVEL3_BUTTON:
								levelSelected = 3;
								m_bStartFadeOut = true;
								SoundManager::playSound("Sounds//select.ogg", false);
							break;

							case LEVEL4_BUTTON:
								levelSelected = 4;
								m_bStartFadeOut = true;
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

void StateLevelSelect::FadeInEffect(double dt)
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

void StateLevelSelect::FadeOutEffect(double dt, StateHandler * stateHandler)
{
	for (Mesh * mesh : m_meshList)
	{
		if (m_meshList[0] && !runOnce)
		{
			runOnce = true;
			m_meshList[1]->alpha = 1.f;
			m_meshList[2]->alpha = 1.f;
		}
		mesh->alpha -= 2.f * dt;
	}

	if (m_meshList[0]->alpha < 0)
	{
		stateHandler->ChangeState(new StateTest("Test Game State", theView, 1, levelSelected));
	}
}