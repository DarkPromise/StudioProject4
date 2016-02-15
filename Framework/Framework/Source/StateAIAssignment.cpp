#include "StateAIAssignment.h"
#include "View.h"
#include "LoadTGA.h"

#include <sstream>

StateAIAssignment::~StateAIAssignment()
{

}

State * StateAIAssignment::getInstance()
{
	return this;
}

void StateAIAssignment::Init()
{
	// AI Assignment is 2D
	theView->LoadOrthoCamera();

	// Init message board
	m_MessageBoard = new MessageBoard();

	// General Meshes
	Mesh * newMesh;
	newMesh = MeshBuilder::GenerateAxes("Axes", 100.f, 100.f, 100.f);
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateText("Source Font", 16, 16);	
	newMesh->textureID = LoadTGA("Fonts//source.tga");
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Base", Color(1.f, 1.f, 1.f), 1.f);
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Waypoint", Color(0.f, 1.f, 0.f), 1.f);
	m_meshList.push_back(newMesh);

	// Create Waypoints and add to list
	Waypoint * newWaypoint;
	newWaypoint = new Waypoint();	// Base Waypoint (Starting Position)
	newWaypoint->position.Set(25.f, theView->getWindowHeight() * 0.5f, 0.f);
	m_waypointList.push_back(newWaypoint);

	newWaypoint = new Waypoint();	// Position Waypoint (Position 1) Index #1
	newWaypoint->position.Set((theView->getWindowWidth() * 0.5f) - 100.f, theView->getWindowHeight() * 0.75f, 0.f);
	m_waypointList.push_back(newWaypoint);

	newWaypoint = new Waypoint();	// Position Waypoint (Position 2) Index #2
	newWaypoint->position.Set((theView->getWindowWidth() * 0.5f) - 100.f, theView->getWindowHeight() * 0.50f, 0.f);
	m_waypointList.push_back(newWaypoint);

	newWaypoint = new Waypoint();	// Position Waypoint (Position 3) Index #3
	newWaypoint->position.Set((theView->getWindowWidth() * 0.5f) - 100.f, theView->getWindowHeight() * 0.25f, 0.f);
	m_waypointList.push_back(newWaypoint);

	newWaypoint = new Waypoint();	// Enemy Waypoint (Starting Position) Index #4
	newWaypoint->position.Set(theView->getWindowWidth() - 150.f, theView->getWindowHeight() * 0.5f, 0.f);
	m_waypointList.push_back(newWaypoint);

	newWaypoint = new Waypoint();	// Wall Waypoint (Position 1) Index #5
	newWaypoint->position.Set((theView->getWindowWidth() * 0.5f) + 100.f, theView->getWindowHeight() * 0.75f, 0.f);
	m_waypointList.push_back(newWaypoint);

	newWaypoint = new Waypoint();	// Wall Waypoint (Position 2) Index #6
	newWaypoint->position.Set((theView->getWindowWidth() * 0.5f) + 100.f, theView->getWindowHeight() * 0.50f, 0.f);
	m_waypointList.push_back(newWaypoint);

	newWaypoint = new Waypoint();	// Wall Waypoint (Position 3) Index #7
	newWaypoint->position.Set((theView->getWindowWidth() * 0.5f) + 100.f, theView->getWindowHeight() * 0.25f, 0.f);
	m_waypointList.push_back(newWaypoint);

	/***************************************************************************************/

	Entity * newEntity;
	newEntity = new EntityWall();
	auto * infoComponent = new InformationComponent();
	infoComponent->setName("Wall");
	infoComponent->setSize(20.f);
	infoComponent->setType(InformationComponent::ENTITY_TYPE::TYPE_NPC);
	newEntity->addComponent(infoComponent);
	auto * graphicsComponent = new GraphicsComponent();
	graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Wall", Color(1.f, 1.f, 1.f), 1.f));
	newEntity->addComponent(graphicsComponent);
	auto * aiComponent = new AIComponent();
	aiComponent->setType(AIComponent::AI_WALL);
	newEntity->addComponent(aiComponent);
	auto * healthComponent = new HealthComponent();
	healthComponent->setHealth(1000.f);
	healthComponent->setMaxHealth(1000.f);
	newEntity->addComponent(healthComponent);
	m_entityList.push_back(newEntity);

	//Create 2 Soldiers
	for (int i = 0; i < 3; i++)
	{
		newEntity = new EntityHero();
		auto * infoComponent = new InformationComponent();
		infoComponent->setName("Soldier(Base)");
		infoComponent->setSize(15.f);
		infoComponent->setType(InformationComponent::ENTITY_TYPE::TYPE_NPC);
		infoComponent->setPosition(m_waypointList[0]->position);
		infoComponent->setAmmoCount(30);
		infoComponent->setMaxAmmo(30);
		newEntity->addComponent(infoComponent);
		auto * graphicsComponent = new GraphicsComponent();
		graphicsComponent->addMesh(MeshBuilder::GenerateQuad("NPC", Color(0.f, 0.f, 1.f), 1.f));
		newEntity->addComponent(graphicsComponent);
		auto * aiComponent = new AIComponent();
		aiComponent->setType(AIComponent::AI_SOLDIER);
		aiComponent->setState(AIComponent::STATE_IDLE);
		aiComponent->setMessageBoard(m_MessageBoard);
		newEntity->addComponent(aiComponent);
		auto * healthComponent = new HealthComponent();
		newEntity->addComponent(healthComponent);
		healthComponent->setHealth(100.f);
		healthComponent->setMaxHealth(100.f);
		auto * waypointComponent = new WaypointComponent();
		waypointComponent->setWaypoints(this->m_waypointList);
		newEntity->addComponent(waypointComponent);
		m_entityList.push_back(newEntity);
	}

	/***************************************************************************************/
}

void StateAIAssignment::Update(StateHandler * stateHandler, double dt)
{
	theView->Update(dt);
}

void StateAIAssignment::HandleEvents(StateHandler * stateHandler)
{

}

void StateAIAssignment::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{

}

void StateAIAssignment::Cleanup()
{
	if (m_MessageBoard)
	{
		delete m_MessageBoard;
	}

	if (!m_waypointList.empty())
	{
		for (int i = 0; i < m_waypointList.size(); i++)
		{
			delete m_waypointList[i];
		}
	}
	m_waypointList.clear();
	m_waypointList.~vector();

	if (!m_entityList.empty())
	{
		for (int i = 0; i < m_entityList.size(); i++)
		{
			delete m_entityList[i];
		}
	}
	m_entityList.clear();
	m_entityList.~vector();

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

void StateAIAssignment::Pause()
{

}

void StateAIAssignment::Resume()
{

}

void StateAIAssignment::Draw(StateHandler * stateHandler)
{
	RenderBase();
	RenderWaypoints();
	RenderEntities();
	RenderMessageBoard();
	theView->SwapBuffers();
}

void StateAIAssignment::RenderEntities()
{
	if (m_entityList.size() > 0)
	{
		for (unsigned int i = 0; i < m_entityList.size(); i++)
		{
			auto infoComponent = m_entityList[i]->getComponent<InformationComponent>();
			auto graphicsComponent = m_entityList[i]->getComponent<GraphicsComponent>();
			auto healthComponent = m_entityList[i]->getComponent<HealthComponent>();
			auto aiComponent = m_entityList[i]->getComponent<AIComponent>();

			std::ostringstream ss;
			ss.precision(0);
			std::ostringstream ss1;
			ss1.precision(0);

			switch(aiComponent->getType())
			{
			case AIComponent::AI_WALL:
				if (healthComponent->getHealth() > 0)
				{
					theView->modelStack.PushMatrix();
					theView->Render2DMesh(graphicsComponent->getMesh(), false, false, infoComponent->getSize(), theView->getWindowHeight(), theView->getWindowWidth() * 0.5f, theView->getWindowHeight() * 0.5f);
					ss << infoComponent->toString() << " Hp(" << healthComponent->getHealth() << "/" << healthComponent->getMaxHealth() << ")";
					theView->RenderTextOnScreen(m_meshList[1], ss.str(), Color(1.f, 1.f, 1.f), 24.f, 15.f + theView->getWindowWidth() * 0.5f, 0.f);
					theView->modelStack.PopMatrix();
				}
				break;
			case AIComponent::AI_ENEMY:
				theView->modelStack.PushMatrix();
				theView->Render2DMesh(graphicsComponent->getMesh(), false, false, infoComponent->getSize(), infoComponent->getSize(), infoComponent->getPosition().x, infoComponent->getPosition().y);
				ss << "Hp(" << healthComponent->getHealth() << "/" << healthComponent->getMaxHealth() << ")";
				theView->RenderTextOnScreen(m_meshList[1], ss.str(), Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 50.f, infoComponent->getPosition().y - 30.f);
				theView->modelStack.PopMatrix();

				switch (aiComponent->getState())
				{
				case AIComponent::STATE_IDLE:
					theView->RenderTextOnScreen(m_meshList[1], "IDLE", Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 20.f, infoComponent->getPosition().y + 10.f);
					break;
				case AIComponent::STATE_KILL:
					theView->RenderTextOnScreen(m_meshList[1], "KILL", Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 20.f, infoComponent->getPosition().y + 10.f);
					break;
				}
				break;
			case AIComponent::AI_SUPPORT:
				theView->modelStack.PushMatrix();
				theView->Render2DMesh(graphicsComponent->getMesh(), false, false, infoComponent->getSize(), infoComponent->getSize(), infoComponent->getPosition().x, infoComponent->getPosition().y);
				theView->RenderTextOnScreen(m_meshList[1], "SUPPORT", Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 35.f, infoComponent->getPosition().y + 10.f);
				theView->modelStack.PopMatrix();
				break;
			default:
				theView->modelStack.PushMatrix();
				theView->Render2DMesh(graphicsComponent->getMesh(), false, false, infoComponent->getSize(), infoComponent->getSize(), infoComponent->getPosition().x, infoComponent->getPosition().y);
				ss << "Hp(" << healthComponent->getHealth() << "/" << healthComponent->getMaxHealth() << ")";
				theView->RenderTextOnScreen(m_meshList[1], ss.str(), Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 50.f, infoComponent->getPosition().y - 30.f);
				ss1 << "Ammo(" << infoComponent->getAmmo() << "/" << infoComponent->getMaxAmmo() << ")";
				theView->RenderTextOnScreen(m_meshList[1], ss1.str(), Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 50.f, infoComponent->getPosition().y - 50.f);
				theView->modelStack.PopMatrix();

				switch (aiComponent->getState())
				{
				case AIComponent::STATE_IDLE:
					theView->RenderTextOnScreen(m_meshList[1],"IDLE", Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 20.f, infoComponent->getPosition().y + 10.f);
					break;
				case AIComponent::STATE_KILL:
					theView->RenderTextOnScreen(m_meshList[1], "KILL", Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 20.f, infoComponent->getPosition().y + 10.f);
					break;
				case AIComponent::STATE_DEFEND:
					theView->RenderTextOnScreen(m_meshList[1], "DEFENDING", Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 45.f, infoComponent->getPosition().y + 10.f);
					break;
				case AIComponent::STATE_SUPPORTING:
					theView->RenderTextOnScreen(m_meshList[1], "HEALING", Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 35.f, infoComponent->getPosition().y + 10.f);
					break;
				case AIComponent::STATE_REQUEST_HELP:
					theView->RenderTextOnScreen(m_meshList[1], "NEED HELP", Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 45.f, infoComponent->getPosition().y + 10.f);
					break;
				case AIComponent::STATE_REQUEST_SUPPLIES:
					theView->RenderTextOnScreen(m_meshList[1], "NEED SUPPLIES", Color(1.f, 1.f, 1.f), 24.f, infoComponent->getPosition().x - 65.f, infoComponent->getPosition().y + 10.f);
					break;
				}

				break;
			}
		}
	}
}

void StateAIAssignment::RenderBase()
{
	theView->Render2DMesh(m_meshList[2], false, false, 100.f, theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.f, (float)theView->getWindowHeight() * 0.5f);
}

void StateAIAssignment::RenderWaypoints()
{
	for (unsigned int i = 0; i < m_waypointList.size(); i++)
	{
		theView->Render2DMesh(m_meshList[3], false, false, 10, 10, m_waypointList[i]->position.x, m_waypointList[i]->position.y);
	}
}

void StateAIAssignment::RenderMessageBoard()
{
	std::ostringstream messageCount;
	messageCount << m_MessageBoard->getMessageBoard().size();

	theView->RenderTextOnScreen(m_meshList[1], "Message Board(" + messageCount.str() + " Messages)", Color(1.f, 1.f, 1.f), 24.f, 50.f, (m_MessageBoard->getMessageBoard().size() * 25.f));
	
	std::ostringstream theMessage;

	if (m_MessageBoard->getMessageBoard().size() > 0)
	{
		for (unsigned int i = 0; i < m_MessageBoard->getMessageBoard().size(); i++)
		{
			switch (m_MessageBoard->getMessageBoard()[i]->type)
			{
			case MESSAGE_HELP:
				theMessage << "From[" << m_MessageBoard->getMessageBoard()[i]->from << "][HELP] Message : " << m_MessageBoard->getMessageBoard()[i]->message;
				break;
			case MESSAGE_SUPPORT:
				theMessage << "From[" << m_MessageBoard->getMessageBoard()[i]->from << "][SUPPORT] Message : " << m_MessageBoard->getMessageBoard()[i]->message;
				break;
			}
			theView->RenderTextOnScreen(m_meshList[1], theMessage.str(), Color(1.f, 1.f, 1.f), 24.f, 50.f, (1 * i) * 25.f);
			theMessage.str("");
			theMessage.clear();
		}
	}
}