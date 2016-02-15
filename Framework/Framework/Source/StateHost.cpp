#include "StateHost.h"
#include "View.h"
#include "LoadTGA.h"

#include "StateMultiplayerMenu.h"

#include "CharBuffer.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include "glfw3.h"
#include "glfw3native.h"


StateHost::~StateHost()
{

}

State * StateHost::getInstance()
{
	return this;
}

void StateHost::Init()
{
	// Menu in 2D
	theView->LoadOrthoCamera();

	// Init the Buffer
	this->m_charBuffer = theView->getInputHandler()->addNewBuffer();
	this->m_charBuffer->setSelected(true);
	this->m_charBuffer->setFontSize(48.f);
	this->m_charBuffer->setHeightOffset(0.5f);
	this->m_charBuffer->setWidthOffset(0.5f);
	this->m_charBuffer->setText("12345");
	this->m_charBuffer->setTextLimit(5);
	this->m_charBuffer->recomputeOffsets();

	// Init the Server
	this->m_cServerManager = new ServerManager(this);
	this->m_cServerManager->setCurrentWindow(glfwGetWin32Window(theView->getWindow()));

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
	newGui = new GuiButton("Start", "Start Server", 0.5f, 0.25f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("StartBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Port", "Click this to change port", 0.5f, 0.45f, 24.f);
	newGui->setTextOffset(120.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("BackBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);

	newGui = new GuiButton("Back(Multiplayer Menu)", "Back", 0.5f, 0.75f, 48.f);
	newGui->setMesh(MeshBuilder::GenerateBoundingBox("BackBB", newGui->getBoundingBox().Max, newGui->getBoundingBox().Min, Color(0.f, 0.f, 1.f)));
	m_guiList.push_back(newGui);
}

void StateHost::Update(StateHandler * stateHandler, double dt)
{
	if (!m_bServerRunning)
	{
		this->m_charBuffer->recomputeOffsets();
		UpdateSelection(stateHandler);
	}
	else
	{
		// Process Server Messages
		m_cServerManager->Select();
	}

	if (m_cServerManager->getEntityList().size() > 0)
	{
		for (unsigned int i = 0; i < m_cServerManager->getEntityList().size(); i++)
		{
			auto infoComponent = m_cServerManager->getEntityList()[i]->getComponent<InformationComponent>();
			infoComponent->Update(dt);
		}
	}
	theView->Update(dt);
}

void StateHost::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_ENTER))
	{
		if (theView->getInputHandler()->getBufferMode())
		{
			theView->getInputHandler()->setBufferMode(false);
			system("cls");
			m_cServerManager->Start("localhost", (unsigned short)strtol(m_charBuffer->toString().c_str(), NULL, 10), ServerManager::NETWORK_TYPE::TYPE_SERVER);
			m_bServerRunning = true;
			
		}
	}
}

void StateHost::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateHost::Cleanup()
{
	if (m_cServerManager != NULL)
	{
		delete m_cServerManager;
	}

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

void StateHost::Pause()
{

}

void StateHost::Resume()
{

}

void StateHost::Draw(StateHandler * stateHandler)
{

	RenderBackground();

	if (!m_bServerRunning)
	{
		RenderButtons();
		RenderBuffer();
	}
	else
	{
		RenderEntities();
	}

	theView->SwapBuffers();
}

void StateHost::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, theView->getWindowWidth(), theView->getWindowHeight(), theView->getWindowWidth() * 0.5f, theView->getWindowHeight() * 0.5f);
}

void StateHost::RenderButtons()
{
	for (unsigned int i = 0; i < m_guiList.size(); i++)
	{
		theView->RenderGui(m_guiList[i], m_meshList[TEXT_FONT]);
	}
}

void StateHost::RenderBuffer()
{
	theView->RenderBuffer(this->m_charBuffer, m_meshList[TEXT_FONT]);
}

void StateHost::RenderEntities()
{
	for (int i = 0; i < m_cServerManager->getEntityList().size(); i++)
	{
		auto graphicsComponent = m_cServerManager->getEntityList()[i]->getComponent<GraphicsComponent>();
		auto infoComponent = m_cServerManager->getEntityList()[i]->getComponent<InformationComponent>();

		theView->modelStack.PushMatrix();
		theView->Render2DMesh(graphicsComponent->getMesh(), false, false, infoComponent->getSize(), infoComponent->getSize(), infoComponent->getPosition().x, infoComponent->getPosition().y, infoComponent->getRotation().z);
		theView->modelStack.PopMatrix();
	}
}

void StateHost::UpdateSelection(StateHandler * stateHandler)
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
						system("cls");
						m_cServerManager->Start("localhost",(unsigned short)strtol(m_charBuffer->toString().c_str(),NULL,10), ServerManager::NETWORK_TYPE::TYPE_SERVER);
						m_bServerRunning = true;
						
						break;
					case PORT_BUTTON:
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

CharBuffer * StateHost::getBuffer()
{
	return this->m_charBuffer;
}

void StateHost::createNewEntity()
{
	Entity * player = new EntityTest();
	auto * infoComponent = new InformationComponent();
	infoComponent->setName("Player");
	infoComponent->setPosition(Vector3(theView->getWindowWidth()*0.5f, theView->getWindowHeight()*0.5f));
	infoComponent->setSize(8.f);
	infoComponent->setType(InformationComponent::ENTITY_TYPE::TYPE_PLAYER);
	player->addComponent(infoComponent);
	auto * graphicsComponent = new GraphicsComponent();
	graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Ship", Color(1.f, 1.f, 1.f), 10.f));
	graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Ship1.tga");
	player->addComponent(graphicsComponent);
	m_cServerManager->getEntityList().push_back(player);
}