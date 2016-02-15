#include "StateAGDevOptions.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateAGDevMenu.h"

StateAGDevOptions::~StateAGDevOptions()
{

}

State * StateAGDevOptions::getInstance()
{
	return this;
}

void StateAGDevOptions::Init()
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
	
	// Create Gui
	Gui * newGui;
	newGui = new GuiButton("On Button", "On", 0.57f, 0.76f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("OnBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Off Button", "Off", 0.57f, 0.76f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("OffBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	m_eCurrentSelection = DEFAULT_BUTTON;
}

void StateAGDevOptions::Update(StateHandler * stateHandler, double dt)
{
	UpdateSelection(stateHandler);
	theView->Update(dt);
}

void StateAGDevOptions::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE))
	{
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView));
	}
}

void StateAGDevOptions::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateAGDevOptions::Cleanup()
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

void StateAGDevOptions::Pause()
{

}

void StateAGDevOptions::Resume()
{

}

void StateAGDevOptions::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	if (SoundManager::getSoundStatus())
	{
		theView->RenderGui(m_guiList[ON_BUTTON],m_meshList[TEXT_FONT]);
	}
	else
	{
		theView->RenderGui(m_guiList[OFF_BUTTON], m_meshList[TEXT_FONT]);
	}
	theView->SwapBuffers();
}

void StateAGDevOptions::UpdateSelection(StateHandler * stateHandler)
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
				if (SoundManager::getSoundStatus())
				{
					m_guiList[ON_BUTTON]->highlightButton(true);
				}
				else if (!SoundManager::getSoundStatus())
				{
					m_guiList[OFF_BUTTON]->highlightButton(true);
				}

				if (theView->getInputHandler()->IsKeyPressed(GLFW_MOUSE_BUTTON_1))
				{
					switch (BUTTON(i))
					{
					case ON_BUTTON:
						SoundManager::toggleSound();
						std::cout << SoundManager::getSoundStatus() << std::endl;
						break;
					case OFF_BUTTON:
						SoundManager::toggleSound();
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

void StateAGDevOptions::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	theView->Render2DMesh(m_meshList[2], false, false, 400.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), 150.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.7f);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Sound : ", Color(1.f, 0.f, 0.f), 48.f, (float)theView->getWindowWidth() * 0.4f, (float)theView->getWindowHeight() * (1.f - 0.76f) - 24.f);
}