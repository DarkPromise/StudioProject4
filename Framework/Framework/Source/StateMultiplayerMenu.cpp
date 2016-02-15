#include "StateMultiplayerMenu.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateMainmenu.h"
#include "StateHost.h"
#include "StateConnect.h"

StateMultiplayerMenu::~StateMultiplayerMenu()
{

}

State * StateMultiplayerMenu::getInstance()
{
	return this;
}

void StateMultiplayerMenu::Init()
{
	// Menu in 2D
	theView->LoadOrthoCamera();

	// Create Meshes
	Mesh * newMesh;
	newMesh = MeshBuilder::GenerateText("Source Font", 16, 16);
	newMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Background", Color(0.f, 0.f, 0.f), 1.f);
	newMesh->textureArray[0] = LoadTGA("Images//MultiplayerBG.tga");
	m_meshList.push_back(newMesh);

	// Create Gui Buttons
	Gui * newGui;
	newGui = new GuiButton("Start", "Start Game", 0.5f, 0.25f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("StartBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Host", "Host Game", 0.5f, 0.5f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("HostBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Main Menu", "Back", 0.5f, 0.75f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("BackBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);
}

void StateMultiplayerMenu::Update(StateHandler * stateHandler, double dt)
{
	UpdateSelection(stateHandler);
	theView->Update(dt);
}

void StateMultiplayerMenu::HandleEvents(StateHandler * stateHandler)
{

}

void StateMultiplayerMenu::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateMultiplayerMenu::Cleanup()
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

void StateMultiplayerMenu::Pause()
{

}

void StateMultiplayerMenu::Resume()
{

}

void StateMultiplayerMenu::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderButtons();
	theView->SwapBuffers();
}

void StateMultiplayerMenu::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, theView->getWindowWidth(), theView->getWindowHeight(), theView->getWindowWidth() * 0.5f, theView->getWindowHeight() * 0.5f);
}

void StateMultiplayerMenu::RenderButtons()
{
	for (unsigned int i = 0; i < m_guiList.size(); i++)
	{
		theView->RenderGui(m_guiList[i], m_meshList[TEXT_FONT]);
	}
}

void StateMultiplayerMenu::UpdateSelection(StateHandler * stateHandler)
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
						stateHandler->ChangeState(new StateConnect("Connect Menu state", this->theView));
						break;
					case HOST_BUTTON:
						stateHandler->ChangeState(new StateHost("Host Menu state", this->theView));
						break;
					case BACK_BUTTON:
						stateHandler->ChangeState(new StateMainmenu("Main Menu state", this->theView));
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