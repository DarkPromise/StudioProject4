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

		if (m_dInputDelay > INTERACTION_DELAY)
		{
			if (m_cInputHandler->IsKeyPressed(GLFW_KEY_E))
			{
				Interact(currMap);
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

void ControllerComponent::Interact(GridMap * currMap)
{
	auto * infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		float indexX = infoC->getPosition().x / (currMap->getMapWidth() * currMap->getTileSize()) * currMap->getMapWidth();
		float indexY = infoC->getPosition().y / (currMap->getMapHeight() * currMap->getTileSize()) * currMap->getMapHeight();
		int playerIndexX = (int)indexX;
		int playerIndexY = currMap->getMapHeight() - (int)indexY;

		// Checking 4 Sides
		if (currMap->getGridMap()[playerIndexY + 1][playerIndexX]->hasInteractableEntity())
		{
			EntityGridObject::OBJECT_TYPE objType = EntityGridObject::OBJECT_TYPE(currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridEntityType());
			switch (objType)
			{
			case EntityGridObject::OBJECT_BOX:
				break;
			case EntityGridObject::OBJECT_KEY:
				break;
			case EntityGridObject::OBJECT_SWITCH:
				currMap->getGridMap()[playerIndexY + 1][playerIndexX]->toggleObjects(currMap);
				break;
			case EntityGridObject::OBJECT_DOOR:
				break;
			}
		}
		if (currMap->getGridMap()[playerIndexY - 1][playerIndexX]->hasInteractableEntity())
		{
			EntityGridObject::OBJECT_TYPE objType = EntityGridObject::OBJECT_TYPE(currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridEntityType());
			switch (objType)
			{
			case EntityGridObject::OBJECT_BOX:
				break;
			case EntityGridObject::OBJECT_KEY:
				break;
			case EntityGridObject::OBJECT_SWITCH:
				currMap->getGridMap()[playerIndexY - 1][playerIndexX]->toggleObjects(currMap);
				break;
			case EntityGridObject::OBJECT_DOOR:
				break;
			}
		}
		if (currMap->getGridMap()[playerIndexY][playerIndexX + 1]->hasInteractableEntity())
		{
			EntityGridObject::OBJECT_TYPE objType = EntityGridObject::OBJECT_TYPE(currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridEntityType());
			switch (objType)
			{
			case EntityGridObject::OBJECT_BOX:
				break;
			case EntityGridObject::OBJECT_KEY:
				break;
			case EntityGridObject::OBJECT_SWITCH:
				currMap->getGridMap()[playerIndexY][playerIndexX + 1]->toggleObjects(currMap);
				break;
			case EntityGridObject::OBJECT_DOOR:
				break;
			}
		}
		if (currMap->getGridMap()[playerIndexY][playerIndexX - 1]->hasInteractableEntity())
		{
			EntityGridObject::OBJECT_TYPE objType = EntityGridObject::OBJECT_TYPE(currMap->getGridMap()[playerIndexY][playerIndexX - 1]->getGridEntityType());
			switch (objType)
			{
			case EntityGridObject::OBJECT_BOX:
				break;
			case EntityGridObject::OBJECT_KEY:
				break;
			case EntityGridObject::OBJECT_SWITCH:
				currMap->getGridMap()[playerIndexY][playerIndexX - 1]->toggleObjects(currMap);
				break;
			case EntityGridObject::OBJECT_DOOR:
				break;
			}
		}
	}
	m_dInputDelay = 0.0;
}