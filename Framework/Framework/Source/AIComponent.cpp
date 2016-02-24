#include "AIComponent.h"
#include "Entity.h"
#include "EntityGridObject.h"
#include "LuaReader.h"

AIComponent::AIComponent()
: m_eState(STATE_IDLE)
, m_eType(AI_UNDEFINED)
, m_eDifficulty(DIFFICULTY_NORMAL)
, m_iSightLength(0.f)
, m_eTargetEntity(nullptr)
, m_cSightMesh(nullptr)
, m_dLookDelay(0.0)
, m_dMoveDelay(0.0)
, m_iMoveCount(0)
{
	
}

AIComponent::~AIComponent()
{
	if (m_cSightMesh)
	{
		delete m_cSightMesh;
	}
}

void AIComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{
	using namespace luabridge;

	auto aiState = tableInfo["aiState"];
	if (aiState.isString())
	{
		if (aiState.cast<std::string>() == "Idle")
		{
			this->setState(STATE_IDLE);
		}
		else if (aiState.cast<std::string>() == "Patrol")
		{
			this->setState(STATE_PATROL);
		}
		else if (aiState.cast<std::string>() == "Chase")
		{
			this->setState(STATE_CHASE);
		}
	}
	else
	{
		std::cout << "AIComponent.aiState for " + name + " is not a string!" << std::endl;
	}
	
	auto aiType = tableInfo["aiType"];
	if (aiType.isString())
	{
		if (aiType.cast<std::string>() == "Undefined")
		{
			this->setType(AI_UNDEFINED);
		}
		else if (aiType.cast<std::string>() == "Guard")
		{
			this->setType(AI_GUARD);
		}
	}
	else
	{
		std::cout << "AIComponent.aiType for " + name + " is not a string!" << std::endl;
	}

	auto aiDifficulty = tableInfo["aiDifficulty"];
	if (aiDifficulty.isString())
	{
		if (aiDifficulty.cast<std::string>() == "Easy")
		{
			this->setDifficulty(DIFFICULTY_EASY);
		}
		else if (aiDifficulty.cast<std::string>() == "Normal")
		{
			this->setDifficulty(DIFFICULTY_NORMAL);
		}
		else if (aiDifficulty.cast<std::string>() == "Hard")
		{
			this->setDifficulty(DIFFICULTY_HARD);
		}
	}
	else
	{
		std::cout << "AIComponent.aiDifficulty for " + name + " is not a string!" << std::endl;
	}

	auto aiSightLength = tableInfo["aiSightLength"];
	if (aiSightLength.isNumber())
	{
		this->setSightLength(aiSightLength.cast<int>());
		m_cSightMesh = MeshBuilder::GenerateLineOfSight("Sight", Color(1.f, 0.f, 0.f), this->getSightLength());
		m_cSightMesh->alpha = 0.5f;
	}
	else
	{
		std::cout << "AIComponent.aiSightLength for " + name + " is not a number!" << std::endl;
	}
}

void AIComponent::Update(double dt, GridMap * gridMap, Entity * thePlayer)
{
	switch (m_eState)
	{
	case STATE_IDLE:
		this->LookAround(dt);
		break;
	case STATE_PATROL:
		this->PatrolAround(gridMap, dt);
		break;
	case STATE_CHASE:
		this->ChaseEntity(gridMap, thePlayer);
		break;
	}
	//After Every Movement, check if the player is within sight radius/chase radius
	this->FindNearbyEntity(gridMap, thePlayer);
	// **Game Over Restart flag 
}

void AIComponent::setType(AI_TYPE type)
{
	this->m_eType = type;
}

AIComponent::AI_TYPE AIComponent::getType()
{
	return this->m_eType;
}

void AIComponent::setState(AI_STATE state)
{
	this->m_eState = state;
}

AIComponent::AI_STATE AIComponent::getState()
{
	return this->m_eState;
}

void AIComponent::setDifficulty(AI_DIFFICULTY difficulty)
{
	this->m_eDifficulty = difficulty;
}

AIComponent::AI_DIFFICULTY AIComponent::getDifficulty()
{
	return this->m_eDifficulty;
}

void AIComponent::setSightLength(int SightLength)
{
	this->m_iSightLength = SightLength;
}

int AIComponent::getSightLength()
{
	return this->m_iSightLength;
}


void AIComponent::FindNearbyEntity(GridMap * gridMap, Entity * entity)
{
	if (m_eState != STATE_IDLE)
	{
		auto infoC = this->getParent()->getComponent<InformationComponent>();
		auto playerInfo = entity->getComponent<InformationComponent>();
		if (infoC)
		{
			float indexX = infoC->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
			float indexY = infoC->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
			int aiIndexX = (int)indexX;
			int aiIndexY = gridMap->getMapHeight() - (int)indexY;

			if (infoC->getDirection().x != 0)
			{
				if (infoC->getDirection().x > 0) // Facing Right
				{
					switch (this->m_iSightLength)
					{
					case 1: // If SightLength is 1, AI Only looks 1 block infront
						if (playerInfo)
						{
							float indexX = playerInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
							float indexY = playerInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
							int playerIndexX = (int)indexX;
							int playerIndexY = gridMap->getMapHeight() - (int)indexY;

							if ((playerIndexX - aiIndexX == this->m_iSightLength) && (playerIndexY - aiIndexY == 0))
							{
								if (playerInfo->getParent()->getComponent<GameplayComponent>())
								{
									playerInfo->getParent()->getComponent<GameplayComponent>()->setRestartLevel(true);
								}
								else
								{
									std::cout << "COULD NOT FIND GAMEPLAY COMPONENT FOR PLAYER" << std::endl;
								}
							}
						}
						break;
					default: // Any Subsequent Length will cause the player to look 3 grids infront
						if (playerInfo)
						{
							float indexX = playerInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
							float indexY = playerInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
							int playerIndexX = (int)indexX;
							int playerIndexY = gridMap->getMapHeight() - (int)indexY;

							for (int i = 0; i < this->m_iSightLength; i++)
							{
								if (((playerIndexX - aiIndexX == (i + 1)) && (playerIndexY - aiIndexY == 0)) || // Right
									((playerIndexX - aiIndexX == (i + 1)) && (aiIndexY - playerIndexY == 1)) || // Top Right
									((playerIndexX - aiIndexX == (i + 1)) && (playerIndexY - aiIndexY == 1))) // Bottom Right
								{
									this->m_eState = STATE_CHASE;
									break;
								}
								else
								{
									this->m_eState = STATE_PATROL;
								}
							}
						}
						break;
					}
				}
				else if (infoC->getDirection().x < 0) // Facing Left
				{
					switch (this->m_iSightLength)
					{
					case 1: // If SightLength is 1, AI Only looks 1 block infront
						if (playerInfo)
						{
							float indexX = playerInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
							float indexY = playerInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
							int playerIndexX = (int)indexX;
							int playerIndexY = gridMap->getMapHeight() - (int)indexY;

							if ((aiIndexX - playerIndexX == this->m_iSightLength) && (aiIndexY - playerIndexY == 0))
							{
								if (playerInfo->getParent()->getComponent<GameplayComponent>())
								{
									playerInfo->getParent()->getComponent<GameplayComponent>()->setRestartLevel(true);
								}
								else
								{
									std::cout << "COULD NOT FIND GAMEPLAY COMPONENT FOR PLAYER" << std::endl;
								}
							}
						}
						break;
					default: // Any Subsequent Length will cause the player to look 3 grids infront
						if (playerInfo)
						{
							float indexX = playerInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
							float indexY = playerInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
							int playerIndexX = (int)indexX;
							int playerIndexY = gridMap->getMapHeight() - (int)indexY;

							for (int i = 0; i < this->m_iSightLength; i++)
							{
								if (((aiIndexX - playerIndexX == (i + 1)) && (aiIndexY - playerIndexY == 0)) || // Left
									((aiIndexX - playerIndexX == (i + 1)) && (playerIndexY - aiIndexY == 1)) || // Bottom Left
									((aiIndexX - playerIndexX == (i + 1)) && (aiIndexY - playerIndexY == 1))) // Top Left
								{
									this->m_eState = STATE_CHASE;
									break;
								}
								else
								{
									this->m_eState = STATE_PATROL;
								}
							}
						}
						break;
					}
				}
			}
			else if (infoC->getDirection().y != 0)
			{
				if (infoC->getDirection().y > 0) // Facing Up
				{
					switch (this->m_iSightLength)
					{
					case 1: // If SightLength is 1, AI Only looks 1 block infront
						if (playerInfo)
						{
							float indexX = playerInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
							float indexY = playerInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
							int playerIndexX = (int)indexX;
							int playerIndexY = gridMap->getMapHeight() - (int)indexY;

							if ((aiIndexX - playerIndexX == 0) && (aiIndexY - playerIndexY == this->m_iSightLength))
							{
								if (playerInfo->getParent()->getComponent<GameplayComponent>())
								{
									playerInfo->getParent()->getComponent<GameplayComponent>()->setRestartLevel(true);
								}
								else
								{
									std::cout << "COULD NOT FIND GAMEPLAY COMPONENT FOR PLAYER" << std::endl;
								}
							}
						}
						break;
					default: // Any Subsequent Length will cause the player to look 3 grids infront
						if (playerInfo)
						{
							float indexX = playerInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
							float indexY = playerInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
							int playerIndexX = (int)indexX;
							int playerIndexY = gridMap->getMapHeight() - (int)indexY;

							for (int i = 0; i < this->m_iSightLength; i++)
							{
								if (((aiIndexX - playerIndexX == 0) && (aiIndexY - playerIndexY == (i + 1))) || // Top
									((playerIndexX - aiIndexX == 1) && (aiIndexY - playerIndexY == (i + 1))) || // Top Right
									((aiIndexX - playerIndexX == 1) && (aiIndexY - playerIndexY == (i + 1)))) // Top Left
								{
									this->m_eState = STATE_CHASE;
									break;
								}
								else
								{
									this->m_eState = STATE_PATROL;
								}
							}
						}
						break;
					}
				}
				else if (infoC->getDirection().y < 0) // Facing Down
				{
					switch (this->m_iSightLength)
					{
					case 1: // If SightLength is 1, AI Only looks 1 block infront
						if (playerInfo)
						{
							float indexX = playerInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
							float indexY = playerInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
							int playerIndexX = (int)indexX;
							int playerIndexY = gridMap->getMapHeight() - (int)indexY;

							if ((playerIndexX - aiIndexX == 0) && (playerIndexY - aiIndexY == this->m_iSightLength))
							{
								if (playerInfo->getParent()->getComponent<GameplayComponent>())
								{
									playerInfo->getParent()->getComponent<GameplayComponent>()->setRestartLevel(true);
								}
								else
								{
									std::cout << "COULD NOT FIND GAMEPLAY COMPONENT FOR PLAYER" << std::endl;
								}
							}
						}
						break;
					default: // Any Subsequent Length will cause the player to look 3 grids infront
						if (playerInfo)
						{
							float indexX = playerInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
							float indexY = playerInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
							int playerIndexX = (int)indexX;
							int playerIndexY = gridMap->getMapHeight() - (int)indexY;

							for (int i = 0; i < this->m_iSightLength; i++)
							{
								if (((playerIndexX - aiIndexX == 0) && (playerIndexY - aiIndexY == (i + 1))) || // Bottom
									((aiIndexX - playerIndexX == 1) && (playerIndexY - aiIndexY == (i + 1))) || // Bottom Left
									((playerIndexX - aiIndexX == 1) && (playerIndexY - aiIndexY == (i + 1)))) // Bottom Right
								{
									this->m_eState = STATE_CHASE;
									break;
								}
								else
								{
									this->m_eState = STATE_PATROL;
								}
							}
						}
						break;
					}
				}
			}
		}
	}
}

void AIComponent::MoveRandomly()
{
	
}

void AIComponent::LookAround(double dt)
{
	m_dLookDelay += dt;
	if (m_eState == STATE_IDLE)
	{
		if (m_dLookDelay > LOOK_DELAY)
		{
			m_dLookDelay = 0.0;
			Math::InitRNG();
			int look = Math::RandIntMinMax(1, 2);
			switch (look)
			{
			case 1: // Look Left
				LookLeft();
				break;
			case 2:
				LookRight();
				break; // Look Right
			}

			auto aiInfo = this->getParent()->getComponent<InformationComponent>();
			if (aiInfo)
			{
				Vector3 dirPos = aiInfo->getDirection() + aiInfo->getPosition();
				float angle = Math::RadianToDegree(atan2(dirPos.x - aiInfo->getPosition().x, dirPos.y - aiInfo->getPosition().y));
				aiInfo->setRotation(angle);
			}
			this->m_eState = STATE_PATROL;
		}
	}
}

void AIComponent::PatrolAround(GridMap * gridMap, double dt)
{
	m_dMoveDelay += dt;
	if (m_dMoveDelay > MOVE_DELAY)
	{
		auto wayC = this->getParent()->getComponent<WaypointComponent>();
		if (wayC)
		{
			auto aiInfo = this->getParent()->getComponent<InformationComponent>();
			if (aiInfo)
			{
				float indexX = aiInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
				float indexY = aiInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
				int aiIndexX = (int)indexX;
				int aiIndexY = gridMap->getMapHeight() - (int)indexY;

				int nextIndex = wayC->getNextWaypointIndex();
				int nextIndexX = wayC->getWaypoints()[nextIndex]->indexX;
				int nextIndexY = wayC->getWaypoints()[nextIndex]->indexY;

				if ((nextIndexX - aiIndexX == 0) && (nextIndexY - aiIndexY == 0))
				{
					if (nextIndex < (wayC->getWaypoints().size() - 1))
					{
						wayC->setNextWaypointIndex(nextIndex + 1);
						m_eState = STATE_IDLE;
					}
					else
					{
						wayC->setNextWaypointIndex(0);
					}
				}
				else
				{
					if (nextIndexX - aiIndexX > 0)
					{
						Vector3 newDirection = Vector3(1, 0, 0);
						aiInfo->setDirection(newDirection);
						// Move Right
						if (gridMap->getGridMap()[aiIndexY][aiIndexX + 1]->getGridEntity() != NULL)
						{
							auto gridObject = dynamic_cast<EntityGridObject*>(gridMap->getGridMap()[aiIndexY][aiIndexX + 1]->getGridEntity());
							if (gridObject)
							{
								if (gridMap->PushObjects(aiIndexX, aiIndexY, GridMap::DIRECTION_RIGHT, gridObject->getObjectType(), this->getParent()))
								{
									aiInfo->setPosition(gridMap->getGridMap()[aiIndexY][aiIndexX + 1]->getGridPos());
									gridMap->getGridMap()[aiIndexY][aiIndexX + 1]->addGridEntity(this->getParent());
									gridMap->getGridMap()[aiIndexY][aiIndexX]->removeEntity();
								}
							}
						}
						else
						{
							aiInfo->setPosition(gridMap->getGridMap()[aiIndexY][aiIndexX + 1]->getGridPos());
							gridMap->getGridMap()[aiIndexY][aiIndexX + 1]->addGridEntity(this->getParent());
							gridMap->getGridMap()[aiIndexY][aiIndexX]->removeEntity();
						}
					}
					else if (nextIndexX - aiIndexX < 0)
					{
						Vector3 newDirection = Vector3(-1, 0, 0);
						aiInfo->setDirection(newDirection);
						// Move Left
						if (gridMap->getGridMap()[aiIndexY][aiIndexX - 1]->getGridEntity() != NULL)
						{
							auto gridObject = dynamic_cast<EntityGridObject*>(gridMap->getGridMap()[aiIndexY][aiIndexX - 1]->getGridEntity());
							if (gridObject)
							{
								if (gridMap->PushObjects(aiIndexX, aiIndexY, GridMap::DIRECTION_LEFT, gridObject->getObjectType(), this->getParent()))
								{
									aiInfo->setPosition(gridMap->getGridMap()[aiIndexY][aiIndexX - 1]->getGridPos());
									gridMap->getGridMap()[aiIndexY][aiIndexX - 1]->addGridEntity(this->getParent());
									gridMap->getGridMap()[aiIndexY][aiIndexX]->removeEntity();
								}
							}
						}
						else
						{
							aiInfo->setPosition(gridMap->getGridMap()[aiIndexY][aiIndexX - 1]->getGridPos());
							gridMap->getGridMap()[aiIndexY][aiIndexX - 1]->addGridEntity(this->getParent());
							gridMap->getGridMap()[aiIndexY][aiIndexX]->removeEntity();
						}
					}
					else if (nextIndexY - aiIndexY > 0)
					{
						Vector3 newDirection = Vector3(0, -1, 0);
						aiInfo->setDirection(newDirection);
						// Move Down
						if (gridMap->getGridMap()[aiIndexY + 1][aiIndexX]->getGridEntity() != NULL)
						{
							auto gridObject = dynamic_cast<EntityGridObject*>(gridMap->getGridMap()[aiIndexY + 1][aiIndexX]->getGridEntity());
							if (gridObject)
							{
								if (gridMap->PushObjects(aiIndexX, aiIndexY, GridMap::DIRECTION_DOWN, gridObject->getObjectType(), this->getParent()))
								{
									aiInfo->setPosition(gridMap->getGridMap()[aiIndexY + 1][aiIndexX]->getGridPos());
									gridMap->getGridMap()[aiIndexY + 1][aiIndexX]->addGridEntity(this->getParent());
									gridMap->getGridMap()[aiIndexY][aiIndexX]->removeEntity();
								}
							}
						}
						else
						{
							aiInfo->setPosition(gridMap->getGridMap()[aiIndexY + 1][aiIndexX]->getGridPos());
							gridMap->getGridMap()[aiIndexY + 1][aiIndexX]->addGridEntity(this->getParent());
							gridMap->getGridMap()[aiIndexY][aiIndexX]->removeEntity();
						}
					}
					else
					{
						Vector3 newDirection = Vector3(0, 1, 0);
						aiInfo->setDirection(newDirection);
						// Move Up
						if (gridMap->getGridMap()[aiIndexY - 1][aiIndexX]->getGridEntity() != NULL)
						{
							auto gridObject = dynamic_cast<EntityGridObject*>(gridMap->getGridMap()[aiIndexY - 1][aiIndexX]->getGridEntity());
							if (gridObject)
							{
								if (gridMap->PushObjects(aiIndexX, aiIndexY, GridMap::DIRECTION_UP, gridObject->getObjectType(), this->getParent()))
								{
									aiInfo->setPosition(gridMap->getGridMap()[aiIndexY - 1][aiIndexX]->getGridPos());
									gridMap->getGridMap()[aiIndexY - 1][aiIndexX]->addGridEntity(this->getParent());
									gridMap->getGridMap()[aiIndexY][aiIndexX]->removeEntity();
								}
							}
						}
						else
						{
							aiInfo->setPosition(gridMap->getGridMap()[aiIndexY - 1][aiIndexX]->getGridPos());
							gridMap->getGridMap()[aiIndexY - 1][aiIndexX]->addGridEntity(this->getParent());
							gridMap->getGridMap()[aiIndexY][aiIndexX]->removeEntity();
						}
					}
				}
				Vector3 dirPos = aiInfo->getDirection() + aiInfo->getPosition();
				float angle = Math::RadianToDegree(atan2(dirPos.x - aiInfo->getPosition().x, dirPos.y - aiInfo->getPosition().y));
				aiInfo->setRotation(angle);
			}
		}
		m_dMoveDelay = 0.0;
	}
}

void AIComponent::RenderLineOfSight(View * theView)
{
	if (m_cSightMesh)
	{
		if (this->getParent()->getComponent<InformationComponent>())
		{
			theView->modelStack.PushMatrix();
			theView->modelStack.Rotate(-this->getParent()->getComponent<InformationComponent>()->getRotation().x, 0.f, 0.f, 1.f);
			theView->RenderMesh(this->m_cSightMesh, false, false);
			theView->modelStack.PopMatrix();
		}
	}
}

void AIComponent::LookLeft()
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		if (infoC->getDirection().x != 0)
		{
			if (infoC->getDirection().x > 0) // Facing Right || Possible Values : Up
			{
				Vector3 newDirection = Vector3(0, 1, 0);
				infoC->setDirection(newDirection);
			}
			else if (infoC->getDirection().x < 0) // Facing Left || Possible Values : Down
			{
				Vector3 newDirection = Vector3(0, -1, 0);
				infoC->setDirection(newDirection);
			}
		}
		else if (infoC->getDirection().y != 0)
		{
			if (infoC->getDirection().y > 0) // Facing Up || Possible Values : Left
			{
				Vector3 newDirection = Vector3(-1, 0, 0);
				infoC->setDirection(newDirection);
			}
			else if (infoC->getDirection().y < 0) // Facing Down || Possible Values : Right
			{
				Vector3 newDirection = Vector3(1, 0, 0);
				infoC->setDirection(newDirection);
			}
		}
	}
}

void AIComponent::LookRight()
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{
		if (infoC->getDirection().x != 0)
		{
			if (infoC->getDirection().x > 0) // Facing Right || Possible Values : Down
			{
				Vector3 newDirection = Vector3(0, -1, 0);
				infoC->setDirection(newDirection);
			}
			else if (infoC->getDirection().x < 0) // Facing Left || Possible Values : Up
			{
				Vector3 newDirection = Vector3(0, 1, 0);
				infoC->setDirection(newDirection);
			}
		}
		else if (infoC->getDirection().y != 0)
		{
			if (infoC->getDirection().y > 0) // Facing Up || Possible Values : Right
			{
				Vector3 newDirection = Vector3(1, 0, 0);
				infoC->setDirection(newDirection);
			}
			else if (infoC->getDirection().y < 0) // Facing Down || Possible Values : Left
			{
				Vector3 newDirection = Vector3(-1, 0, 0);
				infoC->setDirection(newDirection);
			}
		}
	}
}

void AIComponent::ChaseEntity(GridMap * gridMap, Entity * entity)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();
	auto playerInfo = entity->getComponent<InformationComponent>();
	if (infoC)
	{
		float indexX = infoC->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
		float indexY = infoC->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
		int aiIndexX = (int)indexX;
		int aiIndexY = gridMap->getMapHeight() - (int)indexY;
		if (playerInfo)
		{
			float indexX = playerInfo->getPosition().x / (gridMap->getMapWidth() * gridMap->getTileSize()) * gridMap->getMapWidth();
			float indexY = playerInfo->getPosition().y / (gridMap->getMapHeight() * gridMap->getTileSize()) * gridMap->getMapHeight();
			int playerIndexX = (int)indexX;
			int playerIndexY = gridMap->getMapHeight() - (int)indexY;

			int diffX = playerIndexX - aiIndexX;
			int diffY = playerIndexY - aiIndexY;

			std::cout << "Difference X : " << diffX << " Difference Y : " << diffY << std::endl;
		}
	}
}
