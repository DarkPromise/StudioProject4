#include "ControllerComponent.h"
#include "EntityGridObject.h"
#include "glfw3.h"
#include "EntityTest.h"

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

	EntityTest *thePlayer = dynamic_cast<EntityTest*>(this->getParent());
	auto * infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		float indexX = infoC->getPosition().x / (currMap->getMapWidth() * currMap->getTileSize()) * currMap->getMapWidth();
		float indexY = infoC->getPosition().y / (currMap->getMapHeight() * currMap->getTileSize()) * currMap->getMapHeight();
		int playerIndexX = (int)indexX;
		int playerIndexY = currMap->getMapHeight() - (int)indexY;

		// CHECK IF BESIDE DOOR TO CLEAR STAGE
		if (currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getTileID() == Grid::TILE_DOOR_NEXTLEVEL || currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getTileID() == Grid::TILE_DOOR_NEXTLEVEL ||
			currMap->getGridMap()[playerIndexY][playerIndexX - 1]->getTileID() == Grid::TILE_DOOR_NEXTLEVEL || currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getTileID() == Grid::TILE_DOOR_NEXTLEVEL)
		{
			thePlayer->unlockDoorNextLevel = true;
			if (m_cInputHandler->IsKeyPressed('E'))
			{
				// MOVE ON TO NEXT LEVEL
			}
		}

		else
		{
			thePlayer->unlockDoorNextLevel = false;
		}

		// CHECK IF BESIDE SWITCH CAN BE ACTIVATED TO UNLOCK DOOR
		EntityGridObject *theObject;
		if (currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridEntity())
		{
			theObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridEntity());
			if (theObject->getObjectType() == EntityGridObject::OBJECT_SWITCH)
			{
				thePlayer->unlockDoor = true;
			}

			else
			{
				thePlayer->unlockDoor = false;
			}
		}

		else if (currMap->getGridMap()[playerIndexY][playerIndexX - 1]->getGridEntity())
		{
			theObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY][playerIndexX - 1]->getGridEntity());
			if (theObject->getObjectType() == EntityGridObject::OBJECT_SWITCH)
			{
				thePlayer->unlockDoor = true;
			}

			else
			{
				thePlayer->unlockDoor = false;
			}
		}

		else if (currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridEntity())
		{
			theObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridEntity());
			if (theObject->getObjectType() == EntityGridObject::OBJECT_SWITCH)
			{
				thePlayer->unlockDoor = true;
			}

			else
			{
				thePlayer->unlockDoor = false;
			}
		}

		else if (currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridEntity())
		{
			theObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridEntity());
			if (theObject->getObjectType() == EntityGridObject::OBJECT_SWITCH)
			{
				thePlayer->unlockDoor = true;
			}

			else
			{
				thePlayer->unlockDoor = false;
			}
		}

		else
		{
			thePlayer->unlockDoor = false;
		}

		if (m_cInputHandler->IsKeyPressed('E') && thePlayer->unlockDoor)
		{
			currMap->getGridMap()[19][4]->replaceTile(Grid::TILE_FLOOR, BACKGROUND_TILE);
		}

		// CONTROL PLAYER
		if (m_dInputDelay > MOVEMENT_DELAY)
		{
			if (m_cInputHandler->IsKeyPressed(GLFW_KEY_UP))
			{
				MoveForward(currMap);
			}
			else if (m_cInputHandler->IsKeyPressed(GLFW_KEY_LEFT))
			{
				MoveLeft(currMap);
			}
			else if (m_cInputHandler->IsKeyPressed(GLFW_KEY_DOWN))
			{
				MoveBackwards(currMap);
			}
			else if (m_cInputHandler->IsKeyPressed(GLFW_KEY_RIGHT))
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