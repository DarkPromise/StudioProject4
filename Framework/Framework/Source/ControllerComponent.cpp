#include "ControllerComponent.h"
#include "Entity.h"
#include "glfw3.h"

ControllerComponent::ControllerComponent(InputHandler * theInputHandler)
: m_cInputHandler(theInputHandler)
, m_dInputDelay(0.0)
{

}

ControllerComponent::~ControllerComponent()
{

}

void ControllerComponent::Update(double dt, GridMap * currMap)
{
	m_dInputDelay += dt;

	auto * infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		if (m_dInputDelay > MOVEMENT_DELAY)
		{
			if (m_cInputHandler->IsKeyPressed(GLFW_KEY_W))
			{
				MoveForward(currMap);
			}
			else if (m_cInputHandler->IsKeyPressed(GLFW_KEY_A))
			{
				MoveLeft(currMap);
			}
			else if (m_cInputHandler->IsKeyPressed(GLFW_KEY_S))
			{
				MoveBackwards(currMap);
			}
			else if (m_cInputHandler->IsKeyPressed(GLFW_KEY_D))
			{
				MoveRight(currMap);
			}
		}
	}
}

void ControllerComponent::setInputHandler(InputHandler * inputHandler)
{
	this->m_cInputHandler = inputHandler;
}

InputHandler * ControllerComponent::getInputHandler()
{
	return this->m_cInputHandler;
}


void ControllerComponent::setInputDelay(double delay)
{
	this->m_dInputDelay = delay;
}

double ControllerComponent::getInputDelay()
{
	return this->m_dInputDelay;
}

void ControllerComponent::MoveForward(GridMap * currMap)
{
	auto * infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		float indexX = infoC->getPosition().x / (currMap->getMapWidth() * currMap->getTileSize()) * currMap->getMapWidth();
		float indexY = infoC->getPosition().y / (currMap->getMapHeight() * currMap->getTileSize()) * currMap->getMapHeight();
		if (((currMap->getMapHeight() - (int)indexY - 1) >= 0))
		{
			if (currMap->getGridMap()[currMap->getMapHeight() - (int)indexY - 1][(int)indexX]->getTileID() != Grid::TILE_WALL)
			{
				infoC->setPosition(currMap->getGridMap()[currMap->getMapHeight() - (int)indexY - 1][(int)indexX]->getGridPos());
			}
		}
	}
	m_dInputDelay = 0.0;
}

void ControllerComponent::MoveBackwards(GridMap * currMap)
{
	auto * infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		float indexX = infoC->getPosition().x / (currMap->getMapWidth() * currMap->getTileSize()) * currMap->getMapWidth();
		float indexY = infoC->getPosition().y / (currMap->getMapHeight() * currMap->getTileSize()) * currMap->getMapHeight();
		if (((currMap->getMapHeight() - (int)indexY + 1) < currMap->getMapHeight()))
		{
			if (currMap->getGridMap()[currMap->getMapHeight() - (int)indexY + 1][(int)indexX]->getTileID() != Grid::TILE_WALL)
			{
				infoC->setPosition(currMap->getGridMap()[currMap->getMapHeight() - (int)indexY + 1][(int)indexX]->getGridPos());
			}
		}
	}
	m_dInputDelay = 0.0;
}

void ControllerComponent::MoveLeft(GridMap * currMap)
{
	auto * infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		float indexX = infoC->getPosition().x / (currMap->getMapWidth() * currMap->getTileSize()) * currMap->getMapWidth();
		float indexY = infoC->getPosition().y / (currMap->getMapHeight() * currMap->getTileSize()) * currMap->getMapHeight();
		if ((indexX - 1 >= 0))
		{
			if (currMap->getGridMap()[currMap->getMapHeight() - (int)indexY][(int)indexX - 1]->getTileID() != Grid::TILE_WALL)
			{
				infoC->setPosition(currMap->getGridMap()[currMap->getMapHeight() - (int)indexY][(int)indexX - 1]->getGridPos());
			}
		}
	}
	m_dInputDelay = 0.0;
}

void ControllerComponent::MoveRight(GridMap * currMap)
{
	auto * infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		float indexX = infoC->getPosition().x / (currMap->getMapWidth() * currMap->getTileSize()) * currMap->getMapWidth();
		float indexY = infoC->getPosition().y / (currMap->getMapHeight() * currMap->getTileSize()) * currMap->getMapHeight();
		if ((indexX + 1 < currMap->getMapWidth()))
		{
			if (currMap->getGridMap()[currMap->getMapHeight() - (int)indexY][(int)indexX + 1]->getTileID() != Grid::TILE_WALL)
			{
				infoC->setPosition(currMap->getGridMap()[currMap->getMapHeight() - (int)indexY][(int)indexX + 1]->getGridPos());
			}
		}
	}
	m_dInputDelay = 0.0;
}