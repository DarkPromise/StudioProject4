#include "ControllerComponent.h"
#include "EntityGridObject.h"
#include "glfw3.h"

ControllerComponent::ControllerComponent(InputHandler * theInputHandler)
: m_cInputHandler(theInputHandler)
, m_dInputDelay(0.0)
, unlockDoorNextLevel(false)
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
		float indexX = infoC->getPosition().x / (currMap->getMapWidth() * currMap->getTileSize()) * currMap->getMapWidth();
		float indexY = infoC->getPosition().y / (currMap->getMapHeight() * currMap->getTileSize()) * currMap->getMapHeight();
		int playerIndexX = (int)indexX;
		int playerIndexY = currMap->getMapHeight() - (int)indexY;

		// CHECK IF IT'S BESIDE DOOR TO CLEAR STAGE
		if (currMap->getGridMap()[currMap->getMapHeight() - (int)indexY][(int)indexX + 1]->getTileID() == Grid::TILE_DOOR_NEXTLEVEL)
		{
			unlockDoorNextLevel = true;
		}

		else
		{
			unlockDoorNextLevel = false;
		}

		// CONTROL PLAYER
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
		int playerIndexX = (int)indexX;
		int playerIndexY = currMap->getMapHeight() - (int)indexY;

		if (playerIndexY - 1 >= 0)
		{
			if (currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getTileID() == Grid::TILE_FLOOR)
			{
				if (currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridEntity() != nullptr)
				{
					auto gridObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridEntity());
					if (currMap->PushObjects(playerIndexX, playerIndexY, GridMap::DIRECTION_UP, gridObject->getObjectType(),this->getParent()))
					{
						infoC->setPosition(currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridPos());
					}
				}
				else
				{
					infoC->setPosition(currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridPos());
				}
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
		int playerIndexX = (int)indexX;
		int playerIndexY = currMap->getMapHeight() - (int)indexY;

		if ((currMap->getMapHeight() - (int)indexY + 1) < currMap->getMapHeight())
		{
			if (currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getTileID() == Grid::TILE_FLOOR)
			{
				if (currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridEntity() != nullptr)
				{
					auto gridObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridEntity());
					if (currMap->PushObjects(playerIndexX, playerIndexY, GridMap::DIRECTION_DOWN, gridObject->getObjectType(),this->getParent()))
					{
						infoC->setPosition(currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridPos());
					}
				}
				else
				{
					infoC->setPosition(currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridPos());
				}
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
		int playerIndexX = (int)indexX;
		int playerIndexY = currMap->getMapHeight() - (int)indexY;

		if (playerIndexX - 1 >= 0)
		{
			if (currMap->getGridMap()[playerIndexY][playerIndexX-1]->getTileID() == Grid::TILE_FLOOR)
			{
				if (currMap->getGridMap()[playerIndexY][playerIndexX-1]->getGridEntity() != nullptr)
				{
					auto gridObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY][playerIndexX-1]->getGridEntity());
					if (currMap->PushObjects(playerIndexX, playerIndexY, GridMap::DIRECTION_LEFT, gridObject->getObjectType(), this->getParent()))
					{
						infoC->setPosition(currMap->getGridMap()[playerIndexY][playerIndexX-1]->getGridPos());
					}
				}
				else
				{
					infoC->setPosition(currMap->getGridMap()[playerIndexY][playerIndexX-1]->getGridPos());
				}
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
		int playerIndexX = (int)indexX;
		int playerIndexY = currMap->getMapHeight() - (int)indexY;

		if (((int)indexX + 1) < currMap->getMapWidth())
		{
			if (currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getTileID() == Grid::TILE_FLOOR)
			{
				if (currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridEntity() != nullptr)
				{
					auto gridObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridEntity());
					if (currMap->PushObjects(playerIndexX, playerIndexY, GridMap::DIRECTION_RIGHT, gridObject->getObjectType(), this->getParent()))
					{
						infoC->setPosition(currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridPos());
					}
				}
				else
				{
					infoC->setPosition(currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridPos());
				}
			}
		}
	}
	m_dInputDelay = 0.0;
}