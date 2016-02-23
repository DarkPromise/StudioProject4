#include "StateAGDevGame.h"
#include "View.h"
#include "LoadTGA.h"
#include "Tree.h"

#include "StateAGDevMenu.h"
#include <sstream>

#define DOMAIN_SIZE 1000.f
#define WORLD_SIZE Vector3(2000.f,2000.f,2000.f)

StateAGDevGame::~StateAGDevGame()
{

}

State * StateAGDevGame::getInstance()
{
	return this;
}

void StateAGDevGame::Init()
{
	
}

void StateAGDevGame::Update(StateHandler * stateHandler, double dt)
{
	// Spawn Random NPCs
	static double spawnDelay = 0.0;
	spawnDelay += dt;
	if (spawnDelay > 2.0) // Spawn every 2s
	{
		if (m_entityList.size() < SPAWN_LIMIT)
		{
			SpawnNPCs();
		}
		spawnDelay = 0.0;
	}

	// Player Update
	auto infoC = thePlayer->getComponent<InformationComponent>();
	if (infoC)
	{
		infoC->Update(dt);
	}
	
	// Entities Update
	for (Entity * entity : m_entityList)
	{
		auto infoC = entity->getComponent<InformationComponent>();
		if (infoC)
		{
			infoC->Update(dt);
		}
	}

	// Camera Update
	theCamera->Update(dt, theView->getInputHandler());
	auto cameraC = thePlayer->getComponent<CameraComponent>();
	if (cameraC)
	{
		cameraC->Update(dt);
	}

	// Octree Update
	tree->UpdateAIComponents(dt, thePlayer);
	tree->UpdateLevelOfDetails(theCamera->getCameraPos());
	int temp1, temp2;
	tree->checkCollisions(temp1, temp2, m_iScore);

	if (m_bStartFadeOut)
	{
		FadeOutEffect(dt);
	}
	theView->Update(dt);

	theView->viewStack.LoadIdentity();
	theView->viewStack.LookAt(
		theCamera->getCameraPos().x, theCamera->getCameraPos().y, theCamera->getCameraPos().z,
		theCamera->getCameraTarget().x, theCamera->getCameraTarget().y, theCamera->getCameraTarget().z,
		theCamera->getCameraUp().x, theCamera->getCameraUp().y, theCamera->getCameraUp().z
		);
}

void StateAGDevGame::HandleEvents(StateHandler * stateHandler)
{
	// Should be done with "ControllerComponent"
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_W))
	{
		thePlayer->getComponent<InformationComponent>()->setVelocity(MOVE_SPEED);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_S))
	{
		thePlayer->getComponent<InformationComponent>()->setVelocity(-MOVE_SPEED);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_A))
	{
		thePlayer->getComponent<InformationComponent>()->applyRotation(ROTATION_SPEED, 1);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_D))
	{
		thePlayer->getComponent<InformationComponent>()->applyRotation(-ROTATION_SPEED, 1);
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_SPACE))
	{
		auto playerEntity = dynamic_cast<EntityPlayer*>(thePlayer);
		playerEntity->Jump();
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE))
	{
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, false));
	}

	if (thePlayer)
	{
		auto healthC = thePlayer->getComponent<HealthComponent>();
		if (healthC)
		{
			if (healthC->getHealth() <= 0)
			{
				stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, false));
			}
		}
	}
}

void StateAGDevGame::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{
	// This Function is Obsolete
}

void StateAGDevGame::Cleanup()
{
	if (thePlayer)
	{
		delete thePlayer;
	}

	if (theCamera)
	{
		delete theCamera;
	}

	if (tree)
	{
		delete tree;
	}

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

void StateAGDevGame::Pause()
{

}

void StateAGDevGame::Resume()
{

}

void StateAGDevGame::Draw(StateHandler * stateHandler)
{
	RenderSkyplane();
	RenderGround();
	RenderOctree();
	//RenderPlayer(); Now Obsolete because the Octree Renders all Entities
	RenderGUI();
	RenderFadeOverlay();
	theView->SwapBuffers();
}

void StateAGDevGame::RenderFadeOverlay()
{
	if (m_meshList[1]->alpha > 0)
	{
		theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	}
}

void StateAGDevGame::RenderSkyplane()
{
	theView->modelStack.PushMatrix();
	theView->modelStack.Translate(500, 5000, -500);
	theView->RenderMesh(m_meshList[2], false, false);
	theView->modelStack.PopMatrix();
}

void StateAGDevGame::RenderGround()
{
	theView->modelStack.PushMatrix();
	theView->modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	theView->modelStack.Rotate(-90.f, 0.f, 0.f, 1.f);
	theView->RenderMesh(m_meshList[3], false, false);
	theView->modelStack.PopMatrix();
}

void StateAGDevGame::RenderPlayer()
{
	theView->modelStack.PushMatrix();
	theView->modelStack.Translate(this->thePlayer->getComponent<InformationComponent>()->getPosition().x, this->thePlayer->getComponent<InformationComponent>()->getPosition().y, this->thePlayer->getComponent<InformationComponent>()->getPosition().z);
	theView->modelStack.Rotate(this->thePlayer->getComponent<InformationComponent>()->getRotation().y, 0.f, 1.f, 0.f);
	theView->RenderMesh(this->thePlayer->getComponent<GraphicsComponent>()->getMesh(this->thePlayer->getComponent<GraphicsComponent>()->getResLevel()), false, false);
	theView->modelStack.PopMatrix();

	auto collisionC = this->thePlayer->getComponent<CollisionComponent>();
	if (collisionC)
	{
		theView->modelStack.PushMatrix();
		theView->modelStack.Translate(this->thePlayer->getComponent<InformationComponent>()->getPosition().x, this->thePlayer->getComponent<InformationComponent>()->getPosition().y, this->thePlayer->getComponent<InformationComponent>()->getPosition().z);
		theView->modelStack.Rotate(this->thePlayer->getComponent<InformationComponent>()->getRotation().y, 0.f, 1.f, 0.f);
		theView->RenderMesh(collisionC->getMesh(), false, false);
		theView->modelStack.PopMatrix();
	}
}

void StateAGDevGame::RenderOctree()
{
	glLineWidth(3.0f);
	tree->Render(theView);
	glLineWidth(1.0f);
}

void StateAGDevGame::RenderGUI()
{
	std::stringstream ss1;
	ss1.precision(0);
	std::stringstream ss2;
	ss2.precision(0);
	std::stringstream ss3;
	ss3.precision(0);

	ss1 << "Hp[" << thePlayer->getComponent<HealthComponent>()->getHealth() << "/" << thePlayer->getComponent<HealthComponent>()->getMaxHealth() << "]";
	theView->RenderTextOnScreen(m_meshList[0], ss1.str(), Color(1.f, 1.f, 1.f), 48.f, 5.f, 0.f);

	// This was just a test
	ss2 << "Ammo[" << thePlayer->getComponent<InformationComponent>()->getAmmo() << "/" << thePlayer->getComponent<InformationComponent>()->getMaxAmmo() << "]";
	//theView->RenderTextOnScreen(m_meshList[0], ss2.str(), Color(1.f, 1.f, 1.f), 48.f, this->theView->getWindowWidth() - (ss2.str().length() * 20.f), 0.f);

	ss3 << "Score : " << m_iScore;
	theView->RenderTextOnScreen(m_meshList[0], ss3.str(), Color(1.f, 1.f, 1.f), 48.f, (this->theView->getWindowWidth() * 0.5f) - (ss3.str().length() * 10.f), 0.f);
}

void StateAGDevGame::FadeOutEffect(double dt)
{
	if (m_meshList[1]->alpha > 0)
	{
		if (dt < 0.3)
		{
			m_meshList[1]->alpha -= 2.f * dt;
		}
	}
	else
	{
		m_bStartFadeOut = false;
	}
}

void StateAGDevGame::SpawnNPCs()
{
	
}