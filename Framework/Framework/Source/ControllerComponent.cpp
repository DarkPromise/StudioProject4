#include "ControllerComponent.h"
#include "EntityGridObject.h"
#include "glfw3.h"
#include "EntityTest.h"
#include "SoundManager.h"

ControllerComponent::ControllerComponent(InputHandler * theInputHandler)
: m_cInputHandler(theInputHandler)
, m_dInputDelay(0.0)
, m_dMoveDelay(0.0)
, m_sMovement(-1,-1,GridMap::DIRECTION_NONE, Vector3(0.f, 0.f, 0.f), false)
{

}

ControllerComponent::~ControllerComponent()
{

}

void ControllerComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{

}

void ControllerComponent::Update(double dt, GridMap * currMap)
{
	m_dInputDelay += dt;
	m_dMoveDelay += dt;

	EntityTest * thePlayer = dynamic_cast<EntityTest*>(this->getParent());
	auto infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		// CONTROL PLAYER
		if (!this->m_sMovement.m_bIsMoving)
		{

			if (m_dMoveDelay > MOVEMENT_DELAY)
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
		else
		{
			float newX, newY;
			int absX, absY;
			Vector3 prevPosition = infoC->getPosition();
			switch (this->m_sMovement.m_eMoveDirection)
			{
			case GridMap::DIRECTION_UP: // X does not Change
				newY = Math::SmoothDamp(infoC->getPosition().y, this->m_sMovement.m_v3NextPosition.y, 4096.f, 0.001f, 4096.f, dt);
				infoC->setPosition(Vector3(prevPosition.x, newY, 0.f));
				absY = (int)newY;
				if (m_sMovement.m_v3NextPosition.y == absY)
				{
					m_sMovement.m_bIsMoving = false;
					currMap->getGridMap()[m_sMovement.m_iPlayerIndexY - 1][m_sMovement.m_iPlayerIndexX]->addGridEntity(currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX]->getGridEntity());
					currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX]->removeEntity();
				}
				break;
			case GridMap::DIRECTION_DOWN: // X does not Change
				newY = Math::SmoothDamp(infoC->getPosition().y, this->m_sMovement.m_v3NextPosition.y, 4096.f, 0.001f, 4096.f, dt);
				infoC->setPosition(Vector3(prevPosition.x, newY, 0.f));
				absY = (int)newY;
				if (m_sMovement.m_v3NextPosition.y == absY)
				{
					m_sMovement.m_bIsMoving = false;
					currMap->getGridMap()[m_sMovement.m_iPlayerIndexY + 1][m_sMovement.m_iPlayerIndexX]->addGridEntity(currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX]->getGridEntity());
					currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX]->removeEntity();
				}
				break;
			case GridMap::DIRECTION_LEFT: // Y does not Change
				newX = Math::SmoothDamp(infoC->getPosition().x, this->m_sMovement.m_v3NextPosition.x, 4096.f, 0.001f, 4096.f, dt);
				infoC->setPosition(Vector3(newX, prevPosition.y, 0.f));
				absX = (int)newX;
				if (m_sMovement.m_v3NextPosition.x == absX)
				{
					m_sMovement.m_bIsMoving = false;
					currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX - 1]->addGridEntity(currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX]->getGridEntity());
					currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX]->removeEntity();
				}
				break;
			case GridMap::DIRECTION_RIGHT: // Y does not Change
				newX = Math::SmoothDamp(infoC->getPosition().x, this->m_sMovement.m_v3NextPosition.x, 4096.f, 0.001f, 4096.f, dt);
				infoC->setPosition(Vector3(newX, prevPosition.y, 0.f));
				absX = (int)newX;
				if (m_sMovement.m_v3NextPosition.x == absX)
				{
					m_sMovement.m_bIsMoving = false;
					currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX + 1]->addGridEntity(currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX]->getGridEntity());
					currMap->getGridMap()[m_sMovement.m_iPlayerIndexY][m_sMovement.m_iPlayerIndexX]->removeEntity();
				}
				break;
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
	auto infoC = this->getParent()->getComponent<InformationComponent>();
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
				if (currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridEntity() != NULL)
				{
					auto gridObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridEntity());
					if (gridObject)
					{
						if (currMap->PushObjects(playerIndexX, playerIndexY, GridMap::DIRECTION_UP, gridObject->getObjectType(), this->getParent()))
						{
							//currMap->getGridMap()[playerIndexY][playerIndexX]->removeEntity();
							this->m_sMovement = MovementUpdate(
								playerIndexX,
								playerIndexY,
								GridMap::DIRECTION_UP,
								currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridPos(),
								true
								);
						}
					}
				}
				else
				{
					//currMap->getGridMap()[playerIndexY][playerIndexX]->removeEntity();
					this->m_sMovement = MovementUpdate(
						playerIndexX,
						playerIndexY,
						GridMap::DIRECTION_UP,
						currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getGridPos(),
						true
						);
				}
			}
		}
	}
	m_dMoveDelay = 0.0;
}

void ControllerComponent::MoveBackwards(GridMap * currMap)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();
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
				if (currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridEntity() != NULL)
				{
					auto gridObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridEntity());
					if (gridObject)
					{
						if (currMap->PushObjects(playerIndexX, playerIndexY, GridMap::DIRECTION_DOWN, gridObject->getObjectType(), this->getParent()))
						{
							//currMap->getGridMap()[playerIndexY][playerIndexX]->removeEntity();
							this->m_sMovement = MovementUpdate(
								playerIndexX,
								playerIndexY,
								GridMap::DIRECTION_DOWN,
								currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridPos(),
								true
								);
						}
					}
				}
				else
				{
					//currMap->getGridMap()[playerIndexY][playerIndexX]->removeEntity();
					this->m_sMovement = MovementUpdate(
						playerIndexX,
						playerIndexY,
						GridMap::DIRECTION_DOWN,
						currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getGridPos(),
						true
						);
				}
			}
		}
	}

	m_dMoveDelay = 0.0;
}

void ControllerComponent::MoveLeft(GridMap * currMap)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		infoC->setRotation(Vector3(0.f, 180.f, 0.f));

		float indexX = infoC->getPosition().x / (currMap->getMapWidth() * currMap->getTileSize()) * currMap->getMapWidth();
		float indexY = infoC->getPosition().y / (currMap->getMapHeight() * currMap->getTileSize()) * currMap->getMapHeight();
		int playerIndexX = (int)indexX;
		int playerIndexY = currMap->getMapHeight() - (int)indexY;

		if (playerIndexX - 1 >= 0)
		{
			if (currMap->getGridMap()[playerIndexY][playerIndexX-1]->getTileID() == Grid::TILE_FLOOR)
			{
				if (currMap->getGridMap()[playerIndexY][playerIndexX-1]->getGridEntity() != NULL)
				{
					auto gridObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY][playerIndexX-1]->getGridEntity());
					if (gridObject)
					{
						if (currMap->PushObjects(playerIndexX, playerIndexY, GridMap::DIRECTION_LEFT, gridObject->getObjectType(), this->getParent()))
						{
							//currMap->getGridMap()[playerIndexY][playerIndexX]->removeEntity();
							this->m_sMovement = MovementUpdate(
								playerIndexX,
								playerIndexY,
								GridMap::DIRECTION_LEFT,
								currMap->getGridMap()[playerIndexY][playerIndexX-1]->getGridPos(),
								true
								);
						}
					}
				}
				else
				{
					//currMap->getGridMap()[playerIndexY][playerIndexX]->removeEntity();
					this->m_sMovement = MovementUpdate(
						playerIndexX,
						playerIndexY,
						GridMap::DIRECTION_LEFT,
						currMap->getGridMap()[playerIndexY][playerIndexX - 1]->getGridPos(),
						true
						);
				}
			}
		}
	}
	m_dMoveDelay = 0.0;
}

void ControllerComponent::MoveRight(GridMap * currMap)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		infoC->setRotation(Vector3(0.f, 0.f, 0.f));

		float indexX = infoC->getPosition().x / (currMap->getMapWidth() * currMap->getTileSize()) * currMap->getMapWidth();
		float indexY = infoC->getPosition().y / (currMap->getMapHeight() * currMap->getTileSize()) * currMap->getMapHeight();
		int playerIndexX = (int)indexX;
		int playerIndexY = currMap->getMapHeight() - (int)indexY;

		if (((int)indexX + 1) < currMap->getMapWidth())
		{
			if (currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getTileID() == Grid::TILE_FLOOR)
			{
				if (currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridEntity() != NULL)
				{
					auto gridObject = dynamic_cast<EntityGridObject*>(currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridEntity());
					if (gridObject)
					{
						if (currMap->PushObjects(playerIndexX, playerIndexY, GridMap::DIRECTION_RIGHT, gridObject->getObjectType(), this->getParent()))
						{
							//currMap->getGridMap()[playerIndexY][playerIndexX]->removeEntity();
							this->m_sMovement = MovementUpdate(
								playerIndexX,
								playerIndexY,
								GridMap::DIRECTION_RIGHT,
								currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridPos(),
								true
								);
						}
					}
				}
				else
				{
					//currMap->getGridMap()[playerIndexY][playerIndexX]->removeEntity();
					this->m_sMovement = MovementUpdate(
						playerIndexX,
						playerIndexY,
						GridMap::DIRECTION_RIGHT,
						currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getGridPos(),
						true
						);
				}
			}
		}
	}
	m_dMoveDelay = 0.0;
}

void ControllerComponent::Interact(GridMap * currMap)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();
	auto gameC = this->getParent()->getComponent<GameplayComponent>();

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

		// CHECK FOR DOOR TO CLEAR LEVEL
		if (currMap->getGridMap()[playerIndexY + 1][playerIndexX]->getTileID() == Grid::TILE_DOOR_CLEAR || currMap->getGridMap()[playerIndexY - 1][playerIndexX]->getTileID() == Grid::TILE_DOOR_CLEAR ||
			currMap->getGridMap()[playerIndexY][playerIndexX + 1]->getTileID() == Grid::TILE_DOOR_CLEAR || currMap->getGridMap()[playerIndexY][playerIndexX - 1]->getTileID() == Grid::TILE_DOOR_CLEAR)
		{
			if (gameC->getHasKey())
			{
				SoundManager::playSound("Sounds//levelclear.wav", false);
				gameC->setLevelCleared(true);
			}
			
			else
			{
				SoundManager::playSound("Sounds//leveldenied.wav", false);
				gameC->setKeyShowTimer(3.0);
			}
		}
	}
	m_dInputDelay = 0.0;
}