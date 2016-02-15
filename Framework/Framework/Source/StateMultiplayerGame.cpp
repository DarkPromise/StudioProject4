#include "StateMultiplayerGame.h"
#include "View.h"
#include "LoadTGA.h"

StateMultiplayerGame::StateMultiplayerGame(std::string name, View * theView, std::string ipAddress)
: State(name, theView)
{
	this->theView = theView;
	
	m_cServerManager = new ServerManager();

	// Number of split messages (should be 2)
	int count;
	// Split the text

	char *cstr = new char[ipAddress.length() + 1];
	strcpy_s(cstr,sizeof(ipAddress),ipAddress.c_str());
	char ** messageSplited = this->splitText(cstr, ":", &count);

	m_cServerManager->Start(messageSplited[0], (unsigned short)strtol(messageSplited[1], NULL, 10), ServerManager::NETWORK_TYPE::TYPE_CLIENT);

	delete[] cstr;
	pingDelay = 0.0;
}

StateMultiplayerGame::~StateMultiplayerGame()
{

}

State * StateMultiplayerGame::getInstance()
{
	return this;
}

void StateMultiplayerGame::Init()
{
	// Menu in 2D
	theView->LoadOrthoCamera();

	// Init Player Entity
	player = new EntityTest();
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

	// Create Meshes
	Mesh * newMesh;
	newMesh = MeshBuilder::GenerateText("Source Font", 16, 16);
	newMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Background", Color(0.f, 0.f, 0.f), 1.f);
	newMesh->textureArray[0] = LoadTGA("Images//MultiplayerBG.tga");
	m_meshList.push_back(newMesh);
}

void StateMultiplayerGame::Update(StateHandler * stateHandler, double dt)
{
	if (m_cServerManager->getConnected())
	{
		pingDelay += dt;
		if (pingDelay > 0.0) //No delay...
		{
			//Create an information packet and pass it to the server
			PacketInfo packet;
			packet.packetID = PACKET_MOVEMENT;
			auto infoC = player->getComponent<InformationComponent>();
			packet.m_sName = infoC->toString();
			packet.m_v3Position = infoC->getPosition();
			packet.m_v3Velocity = infoC->getVelocity();
			packet.m_v3Direction = infoC->getDirection();
			packet.m_fSize = infoC->getSize();
			packet.m_v3Rotation = infoC->getRotation();
			m_cServerManager->Ping(packet, this);
		}
	}
	else
	{
		//Forcing Exit
		theView->getInputHandler()->PressKey(GLFW_KEY_ESCAPE, true);
	}
	
	if (player != NULL)
	{
		auto infoComponent = player->getComponent<InformationComponent>();
		if (infoComponent != NULL)
		{
			infoComponent->Update(dt);
		}
	}

	if (otherPlayer != NULL)
	{
		auto infoComponent = otherPlayer->getComponent<InformationComponent>();
		if (infoComponent != NULL)
		{
			infoComponent->Update(dt);
		}
	}

	if (m_entityList.size() > 1)
	{
		for (unsigned int i = 0; i < m_entityList.size(); i++)
		{
			auto infoComponent = m_entityList[i]->getComponent<InformationComponent>();
			infoComponent->Update(dt);
			if ((infoComponent->getPosition().x > theView->getWindowWidth() + 100.f) ||
				(infoComponent->getPosition().y > theView->getWindowHeight() + 100.f)||
				(infoComponent->getPosition().x < 0) || (infoComponent->getPosition().y < 0))
			{
				delete m_entityList[i];
				m_entityList.erase(m_entityList.begin() + i);
			}
		}
	}

	theView->Update(dt);
}

void StateMultiplayerGame::HandleEvents(StateHandler * stateHandler)
{
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_UP))
	{
		auto infoComponent = player->getComponent<InformationComponent>();
		//infoComponent->setForce(infoComponent->getForce() + 1000.f);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_DOWN))
	{
		auto infoComponent = player->getComponent<InformationComponent>();
		//infoComponent->setForce(infoComponent->getForce() - 1000.f);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_LEFT))
	{
		auto infoComponent = player->getComponent<InformationComponent>();
		infoComponent->setRotation(infoComponent->getRotation() + 4.f);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_RIGHT))
	{
		auto infoComponent = player->getComponent<InformationComponent>();
		infoComponent->setRotation(infoComponent->getRotation() - 4.f);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_SPACE))
	{
		// Create Bullets
		Entity * newEntity = new EntityTest();
		auto * infoComponent = new InformationComponent();
		infoComponent->setName("Bullet");
		infoComponent->setPosition(player->getComponent<InformationComponent>()->getPosition());
		infoComponent->setDirection(player->getComponent<InformationComponent>()->getDirection());
		infoComponent->setRotation(player->getComponent<InformationComponent>()->getRotation());
		Vector3 bulletVel = Vector3(0.f, 0.f, 0.f);
		bulletVel.x = player->getComponent<InformationComponent>()->getDirection().x * 200 * 1.2;
		bulletVel.y = player->getComponent<InformationComponent>()->getDirection().y * 200 * 1.2;
		infoComponent->setVelocity(bulletVel);
		infoComponent->setSize(4.f);
		infoComponent->setType(InformationComponent::ENTITY_TYPE::TYPE_PROJ);
		newEntity->addComponent(infoComponent);
		auto * graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Bullet", Color(1.f, 1.f, 1.f), 2.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//GreenBullet.tga");
		newEntity->addComponent(graphicsComponent);
		m_entityList.push_back(newEntity);
	}
}

void StateMultiplayerGame::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateMultiplayerGame::Cleanup()
{
	if (!m_entityList.empty())
	{
		for (int i = 0; i < m_entityList.size(); i++)
		{
			delete m_entityList[i];
		}
	}
	m_entityList.clear();
	m_entityList.~vector();

	if (player != NULL)
	{
		delete player;
	}
	if (otherPlayer != NULL)
	{
		delete otherPlayer;
	}

	if (m_cServerManager != NULL)
	{
		delete m_cServerManager;
	}

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

void StateMultiplayerGame::Pause()
{

}

void StateMultiplayerGame::Resume()
{

}

void StateMultiplayerGame::Draw(StateHandler * stateHandler)
{
	RenderBackground();
	RenderEntities();
	RenderPlayer();
	theView->SwapBuffers();
}

void StateMultiplayerGame::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, theView->getWindowWidth(), theView->getWindowHeight(), theView->getWindowWidth() * 0.5f, theView->getWindowHeight() * 0.5f);
}

void StateMultiplayerGame::RenderPlayer()
{
	auto graphicsComponent = player->getComponent<GraphicsComponent>();
	auto infoComponent = player->getComponent<InformationComponent>();
	theView->modelStack.PushMatrix();
	theView->Render2DMesh(graphicsComponent->getMesh(), false, false, infoComponent->getSize(), infoComponent->getSize(), infoComponent->getPosition().x, infoComponent->getPosition().y,infoComponent->getRotation().z);
	theView->modelStack.PopMatrix();

	if (otherPlayer != NULL)
	{
		auto graphicsComponent2 = otherPlayer->getComponent<GraphicsComponent>();
		auto infoComponent2 = otherPlayer->getComponent<InformationComponent>();
		theView->modelStack.PushMatrix();
		theView->Render2DMesh(graphicsComponent2->getMesh(), false, false, infoComponent2->getSize(), infoComponent2->getSize(), infoComponent2->getPosition().x, infoComponent2->getPosition().y, infoComponent2->getRotation().z);
		theView->modelStack.PopMatrix();
	}
}

void StateMultiplayerGame::RenderEntities()
{
	if (m_entityList.size() > 1)
	{
		for (unsigned int i = 0; i < m_entityList.size(); i++)
		{
			auto graphicsComponent = m_entityList[i]->getComponent<GraphicsComponent>();
			auto infoComponent = m_entityList[i]->getComponent<InformationComponent>();

			theView->modelStack.PushMatrix();
			theView->Render2DMesh(graphicsComponent->getMesh(), false, false, infoComponent->getSize() * 1, infoComponent->getSize() * 5, infoComponent->getPosition().x, infoComponent->getPosition().y, infoComponent->getRotation().z);
			theView->modelStack.PopMatrix();
		}
	}
}

void StateMultiplayerGame::createNewEntity()
{
	Entity * newPlayer = new EntityTest();
	auto * infoComponent = new InformationComponent();
	infoComponent->setName("Player");
	infoComponent->setPosition(Vector3(theView->getWindowWidth()*0.5f, theView->getWindowHeight()*0.5f));
	infoComponent->setSize(8.f);
	infoComponent->setType(InformationComponent::ENTITY_TYPE::TYPE_PLAYER);
	newPlayer->addComponent(infoComponent);
	auto * graphicsComponent = new GraphicsComponent();
	graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Ship", Color(1.f, 1.f, 1.f), 10.f));
	graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Ship1.tga");
	newPlayer->addComponent(graphicsComponent);
	this->otherPlayer = newPlayer;
}

char** StateMultiplayerGame::splitText(char *string, char *delim, int *count)
{
	char **retval = NULL;
	char *token = NULL;
	char *context = NULL;

	*count = 0;
	token = strtok_s(string, delim, &context);

	while (token != NULL)
	{
		*count = *count + 1;
		retval = (char**)realloc(retval, (*count * sizeof(char*)));
		retval[*count - 1] = _strdup(token);
		token = strtok_s(NULL, delim, &context);
	}

	return retval;
}