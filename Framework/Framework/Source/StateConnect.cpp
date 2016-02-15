#include "StateConnect.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateMultiplayerMenu.h"
#include "StateMultiplayerGame.h"

#include "CharBuffer.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include "glfw3.h"
#include "glfw3native.h"

StateConnect::~StateConnect()
{

}

State * StateConnect::getInstance()
{
	return this;
}

void StateConnect::Init()
{
	// Menu in 2D
	theView->LoadOrthoCamera();

	// Init the Buffer
	this->m_charBuffer = theView->getInputHandler()->addNewBuffer();
	this->m_charBuffer->setSelected(true);
	this->m_charBuffer->setFontSize(48.f);
	this->m_charBuffer->setHeightOffset(0.5f);
	this->m_charBuffer->setWidthOffset(0.5f);
	this->m_charBuffer->setText("127.0.0.1:12345");
	this->m_charBuffer->setTextLimit(25);
	this->m_charBuffer->recomputeOffsets();

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

	newGui = new GuiButton("Address", "Click this to change IP", 0.5f, 0.45f, 24.f);
	newGui->setTextOffset(120.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("BackBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Back(Multiplayer Menu)", "Back", 0.5f, 0.75f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("BackBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);
}

void StateConnect::Update(StateHandler * stateHandler, double dt)
{
	this->m_charBuffer->recomputeOffsets();
	UpdateSelection(stateHandler);

	theView->Update(dt);
}

void StateConnect::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_ENTER))
	{
		if (theView->getInputHandler()->getBufferMode())
		{
			theView->getInputHandler()->setBufferMode(false);
			stateHandler->ChangeState(new StateMultiplayerGame("Game State", theView, m_charBuffer->toString()));
		}
	}
}

void StateConnect::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateConnect::Cleanup()
{
	if (m_charBuffer)
	{
		this->m_charBuffer = NULL;
	}

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

void StateConnect::Pause()
{

}

void StateConnect::Resume()
{

}

void StateConnect::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderButtons();
	RenderBuffer();
	theView->SwapBuffers();
}

void StateConnect::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, theView->getWindowWidth(), theView->getWindowHeight(), theView->getWindowWidth() * 0.5f, theView->getWindowHeight() * 0.5f);
}

void StateConnect::RenderButtons()
{
	for (unsigned int i = 0; i < m_guiList.size(); i++)
	{
		theView->RenderGui(m_guiList[i], m_meshList[TEXT_FONT]);
	}
}

void StateConnect::RenderBuffer()
{
	theView->RenderBuffer(this->m_charBuffer, m_meshList[TEXT_FONT]);
}

void StateConnect::UpdateSelection(StateHandler * stateHandler)
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
						theView->getInputHandler()->setBufferMode(false);
						this->m_charBuffer->setHighlighted(false);
						stateHandler->ChangeState(new StateMultiplayerGame("Game State", theView, m_charBuffer->toString()));
						break;
					case IP_BUTTON:
						theView->getInputHandler()->setBufferMode(true);
						this->m_charBuffer->setHighlighted(true);
						break;
					case BACK_BUTTON:
						theView->getInputHandler()->setBufferMode(false);
						this->m_charBuffer->setHighlighted(false);
						stateHandler->ChangeState(new StateMultiplayerMenu("Multiplayer Menu state", this->theView));
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

CharBuffer * StateConnect::getBuffer()
{
	return this->m_charBuffer;
}