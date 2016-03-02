#include "GridMap.h"
#include "EntityGridObject.h"
#include "EntityTest.h"
#include "SoundManager.h"

GridMap::GridMap()
: m_iTileSize(0)
, m_iNumTilesWidth(0)
, m_iNumTilesHeight(0)
{

}

GridMap::~GridMap()
{
	backgroundData.clear();
	backgroundData.~vector();
	foregroundData.clear();
	foregroundData.~vector();
	collisionData.clear();
	collisionData.~vector();

	for (unsigned int i = 0; i < m_iNumTilesHeight; i++)
	{
		for (unsigned int j = 0; j < m_iNumTilesWidth; j++)
		{
			delete m_cGridMap[i][j];
		}
	}
	m_cGridMap.clear();
	m_cGridMap.~vector();
}

void GridMap::Init(int xSize, int ySize, int tileSize)
{
	m_iNumTilesWidth = xSize;
	m_iNumTilesHeight = ySize;
	m_iTileSize = tileSize;

	Grid * newGrid;

	// Set the size of the vectors
	m_cGridMap.resize(ySize);
	for (unsigned int i = 0; i < ySize; i++)
	{
		m_cGridMap[i].resize(xSize);
	}

	// Populate the vector array with empty Grids
	for (unsigned int i = 0; i < ySize; i++)
	{
		for (unsigned int j = 0; j < xSize; j++)
		{
			newGrid = new Grid(tileSize);
			newGrid->setGridPos(Vector3(j * tileSize, (ySize - i) * tileSize, 0.f)); // ySize - i to flip it around
			m_cGridMap[i][j] = newGrid;
		}
	}

	backgroundData.resize(ySize);
	for (unsigned int i = 0; i < ySize; ++i)
	{
		backgroundData[i].resize(xSize);
	}

	foregroundData.resize(ySize);
	for (unsigned int i = 0; i < ySize; ++i)
	{
		foregroundData[i].resize(xSize);
	}

	collisionData.resize(ySize);
	for (unsigned int i = 0; i < ySize; ++i)
	{

		collisionData[i].resize(xSize);
	}
}

void GridMap::Update(double dt)
{

}

void GridMap::RenderGrids(View * theView, Mesh * textMesh, bool renderBB)
{
	for (unsigned int i = 0; i < m_cGridMap.size(); i++)
	{
		for (unsigned int j = 0; j < m_cGridMap[i].size(); j++)
		{
			if (renderBB)
			{
				theView->modelStack.PushMatrix();
				theView->modelStack.Translate(m_cGridMap[i][j]->getGridPos().x, m_cGridMap[i][j]->getGridPos().y, 0);
				theView->RenderMesh(m_cGridMap[i][j]->getMesh(BOUNDING_BOX), false, false);
				theView->modelStack.PopMatrix();
			}
		}
	}
}

void GridMap::RenderLevel(View * theView)
{
	for (unsigned int i = 0; i < m_cGridMap.size(); i++)
	{
		for (unsigned int j = 0; j < m_cGridMap[i].size(); j++)
		{
			if (m_cGridMap[i][j]->getGridTiles().size() > 1)
			{
				theView->modelStack.PushMatrix();
				theView->modelStack.Translate(m_cGridMap[i][j]->getGridPos().x, m_cGridMap[i][j]->getGridPos().y, m_cGridMap[i][j]->getGridPos().z);
				theView->RenderMesh(m_cGridMap[i][j]->getMesh(BACKGROUND_TILE), false, false);
				theView->modelStack.PopMatrix();
			}
		}
	}
}

void GridMap::RenderForeground(View * theView)
{
	for (unsigned int i = 0; i < m_cGridMap.size(); i++)
	{
		for (unsigned int j = 0; j < m_cGridMap[i].size(); j++)
		{
			if (m_cGridMap[i][j]->getGridTiles().size() > 2)
			{
				theView->modelStack.PushMatrix();
				theView->modelStack.Translate(m_cGridMap[i][j]->getGridPos().x, m_cGridMap[i][j]->getGridPos().y, 0.1f);
				theView->RenderMesh(m_cGridMap[i][j]->getMesh(FOREGROUND_TILE), false, false);
				theView->modelStack.PopMatrix();
			}
		}
	}
}

void GridMap::RenderGridEntities(View * theView)
{
	for (unsigned int i = 0; i < m_cGridMap.size(); i++)
	{
		for (unsigned int j = 0; j < m_cGridMap[i].size(); j++)
		{
			if (m_cGridMap[i][j]->getGridEntity() != NULL)
			{
				if (m_cGridMap[i][j]->getGridEntity()->getComponent<ControllerComponent>())
				{

				}
				else
				{
					theView->modelStack.PushMatrix();
					theView->modelStack.Translate(m_cGridMap[i][j]->getGridPos().x, m_cGridMap[i][j]->getGridPos().y, 0.1f);
					theView->RenderMesh(m_cGridMap[i][j]->getGridEntity()->getComponent<GraphicsComponent>()->getMesh(), false, false);
					theView->modelStack.PopMatrix();
				}
			}
		}
	}
}

bool GridMap::PushObjects(int pIndexX, int pIndexY, int direction, int EntityType, Entity * entity)
{
	auto gameC = entity->getComponent<GameplayComponent>();

	switch (GRID_DIRECTION(direction))
	{
		case DIRECTION_UP:
		{
			switch (EntityGridObject::OBJECT_TYPE(EntityType))
			{
				case EntityGridObject::OBJECT_BOX:
					// If 2 Spaces Above is Empty and 2 Spaces above is not a Wall.
					if ((m_cGridMap[pIndexY - 2][pIndexX]->getGridEntity() == NULL) && (m_cGridMap[pIndexY - 2][pIndexX]->getTileID() == Grid::TILE_FLOOR))
					{
						if (m_cGridMap[pIndexY - 1][pIndexX]->getGridEntity())
						{
							SoundManager::playSound("Sounds//slide.wav", false);
							m_cGridMap[pIndexY - 2][pIndexX]->addGridEntity(m_cGridMap[pIndexY - 1][pIndexX]->getGridEntity());
							m_cGridMap[pIndexY - 1][pIndexX]->removeEntity();
							return true;
						}
					}
				break;
			
				case EntityGridObject::OBJECT_KEY:
					if (gameC)
					{
						if (m_cGridMap[pIndexY - 1][pIndexX]->getGridEntity())
						{
							SoundManager::playSound("Sounds//collect.wav", false);
							m_cGridMap[pIndexY - 1][pIndexX]->deleteEntity();
							gameC->setHasKey(true);
							return true;
						}
					}
				break;
			}
		}
		break;
		
		case DIRECTION_DOWN:
		{
			switch (EntityGridObject::OBJECT_TYPE(EntityType))
			{
				case EntityGridObject::OBJECT_BOX:
					// If 2 Spaces Above is Empty and 2 Spaces above is not a Wall.
					if ((m_cGridMap[pIndexY + 2][pIndexX]->getGridEntity() == NULL) && (m_cGridMap[pIndexY + 2][pIndexX]->getTileID() == Grid::TILE_FLOOR))
					{
						if (m_cGridMap[pIndexY + 1][pIndexX]->getGridEntity())
						{
							SoundManager::playSound("Sounds//slide.wav", false);
							m_cGridMap[pIndexY + 2][pIndexX]->addGridEntity(m_cGridMap[pIndexY + 1][pIndexX]->getGridEntity());
							m_cGridMap[pIndexY + 1][pIndexX]->removeEntity();
							return true;
						}
					}
				break;
			
				case EntityGridObject::OBJECT_KEY:
					if (gameC)
					{
						if (m_cGridMap[pIndexY + 1][pIndexX]->getGridEntity())
						{
							SoundManager::playSound("Sounds//collect.wav", false);
							m_cGridMap[pIndexY + 1][pIndexX]->deleteEntity();
							gameC->setHasKey(true);
							return true;
						}
					}
				break;
			}
		}
		break;
		
		case DIRECTION_LEFT:
		{
			switch (EntityGridObject::OBJECT_TYPE(EntityType))
			{
				case EntityGridObject::OBJECT_BOX:
					if ((m_cGridMap[pIndexY][pIndexX - 2]->getGridEntity() == NULL) && (m_cGridMap[pIndexY][pIndexX - 2]->getTileID() == Grid::TILE_FLOOR))
					{
						if (m_cGridMap[pIndexY][pIndexX - 1]->getGridEntity())
						{
							SoundManager::playSound("Sounds//slide.wav", false);
							m_cGridMap[pIndexY][pIndexX - 2]->addGridEntity(m_cGridMap[pIndexY][pIndexX - 1]->getGridEntity());
							m_cGridMap[pIndexY][pIndexX - 1]->removeEntity();
							return true;
						}
					}
				break;
			
				case EntityGridObject::OBJECT_KEY:
					if (gameC)
					{
						if (m_cGridMap[pIndexY][pIndexX - 1]->getGridEntity())
						{
							SoundManager::playSound("Sounds//collect.wav", false);
							m_cGridMap[pIndexY][pIndexX - 1]->deleteEntity();
							gameC->setHasKey(true);
							return true;
						}
					}
				break;
			}
		}
		break;
		
		case DIRECTION_RIGHT:
		{
			switch (EntityGridObject::OBJECT_TYPE(EntityType))
			{
				case EntityGridObject::OBJECT_BOX:
					if ((m_cGridMap[pIndexY][pIndexX + 2]->getGridEntity() == NULL) && (m_cGridMap[pIndexY][pIndexX + 2]->getTileID() == Grid::TILE_FLOOR))
					{
						if (m_cGridMap[pIndexY][pIndexX + 1]->getGridEntity())
						{
							SoundManager::playSound("Sounds//slide.wav", false);
							m_cGridMap[pIndexY][pIndexX + 2]->addGridEntity(m_cGridMap[pIndexY][pIndexX + 1]->getGridEntity());
							m_cGridMap[pIndexY][pIndexX + 1]->removeEntity();
							return true;
						}
					}
				break;
			
				case EntityGridObject::OBJECT_KEY:
					if (gameC)
					{
						if (m_cGridMap[pIndexY][pIndexX + 1]->getGridEntity())
						{
							SoundManager::playSound("Sounds//collect.wav", false);
							m_cGridMap[pIndexY][pIndexX + 1]->deleteEntity();
							gameC->setHasKey(true);
							return true;
						}
					}
				break;
			}
		}
		break;
	} // End of first switch statement
	return false;
}

void GridMap::addGridEntity(Entity * entity)
{
	auto infoC = entity->getComponent<InformationComponent>();
	if (infoC)
	{
		float indexX = infoC->getPosition().x / (this->getMapWidth() * this->getTileSize()) * this->getMapWidth();
		float indexY = infoC->getPosition().y / (this->getMapHeight() * this->getTileSize()) * this->getMapHeight();
		int objIndexX = (int)indexX;
		int objIndexY = this->getMapHeight() - (int)indexY;
		this->m_cGridMap[objIndexY][objIndexX]->addGridEntity(entity);
	}
}

void GridMap::removeGridEntity(Entity * entity)
{
	auto infoC = entity->getComponent<InformationComponent>();
	if (infoC)
	{
		float indexX = infoC->getPosition().x / (this->getMapWidth() * this->getTileSize()) * this->getMapWidth();
		float indexY = infoC->getPosition().y / (this->getMapHeight() * this->getTileSize()) * this->getMapHeight();
		int objIndexX = (int)indexX;
		int objIndexY = this->getMapHeight() - (int)indexY;
		this->m_cGridMap[objIndexY][objIndexX]->removeEntity();
	}
}

std::vector<std::vector<Grid*>> GridMap::getGridMap()
{
	return this->m_cGridMap;
}

bool GridMap::LoadData(std::string backgroundCSV)
{
	if (LoadBackgroundMap(backgroundCSV))
	{
		std::cout << std::endl;
		std::cout << backgroundCSV << " has been loaded. " << std::endl;
		return true;
	}
	else
	{
		std::cout << "Failed to load background" << std::endl;
		return false;
	}
	return false;
}

bool GridMap::LoadBackgroundMap(const std::string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;
	int Index = -1;

	std::ifstream file(mapName.c_str());
	if (file.is_open())
	{
		int i = 0, k = 0;
		while (file.good())
		{
			std::string aLineOfText = "";
			getline(file, aLineOfText);

			if (theLineCounter > m_iNumTilesHeight)
				break;

			// If this line is not a comment line, then process it
			if (!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				if (theLineCounter == 0)
				{
					// This is the first line of the map data file
					std::string token;
					std::istringstream iss(aLineOfText);
					while (getline(iss, token, ','))
					{
						// Count the number of columns
						theMaxNumOfColumns = atoi(token.c_str());
					}
					if (theMaxNumOfColumns != m_iNumTilesWidth)
						return false;
				}
				else
				{
					int theColumnCounter = 0;

					std::string token;
					std::istringstream iss(aLineOfText);
					while (getline(iss, token, ',') && (theColumnCounter < m_iNumTilesWidth))
					{
						if (atoi(token.c_str()) > 0)
						{
							int tileValue = atoi(token.c_str());
							EntityGridObject * newGridObject;
							auto graphicsComponent = new GraphicsComponent();
							switch (tileValue)
							{
							case Grid::TILE_BOX:
								newGridObject = new EntityGridObject(EntityGridObject::OBJECT_BOX);
								graphicsComponent->addMesh(MeshBuilder::GenerateQuad("Box", Color(1.f, 0.f, 0.f), 32.f));
								graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile82.tga");
								newGridObject->addComponent(graphicsComponent);
								this->m_cGridMap[Index + 1][theColumnCounter]->addGridEntity(newGridObject);
								this->m_cGridMap[Index + 1][theColumnCounter]->addTile(Grid::TILE_FLOOR);
								break;
							default:
								delete graphicsComponent;
								this->m_cGridMap[Index + 1][theColumnCounter]->addTile(atoi(token.c_str()));
								break;
							}
						}
						backgroundData[Index+1][theColumnCounter++] = atoi(token.c_str());
					}
					Index++;
				}
			}
			theLineCounter++;
		}
	}
	return true;
}

bool GridMap::LoadForegroundMap(const std::string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;
	int Index = -1;

	std::ifstream file(mapName.c_str());
	if (file.is_open())
	{
		int i = 0, k = 0;
		while (file.good())
		{
			std::string aLineOfText = "";
			getline(file, aLineOfText);

			if (theLineCounter >= m_iNumTilesHeight)
			{
				break;
			}

			// If this line is not a comment line, then process it
			if (!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				if (theLineCounter == 0)
				{
					// This is the first line of the map data file
					std::string token;
					std::istringstream iss(aLineOfText);
					while (getline(iss, token, ','))
					{
						// Count the number of columns
						theMaxNumOfColumns = atoi(token.c_str());
					}
					if (theMaxNumOfColumns != m_iNumTilesWidth)
					{
						std::cout << theMaxNumOfColumns << std::endl;
						return false;

					}
				}
				else
				{
					int theColumnCounter = 0;
					std::string token;
					std::istringstream iss(aLineOfText);
					while (getline(iss, token, ',') && (theColumnCounter<m_iNumTilesWidth))
					{
						if (atoi(token.c_str()) > 0)
						{
							this->m_cGridMap[Index + 1][theColumnCounter]->addTile(atoi(token.c_str()));
						}
						foregroundData[Index+1][theColumnCounter++] = atoi(token.c_str());
					}
					Index++;
				}
			}

			theLineCounter++;
		}
	}
	return true;
}

void GridMap::setTileSize(int tileSize)
{
	this->m_iTileSize = tileSize;
}

int GridMap::getTileSize()
{
	return this->m_iTileSize;
}

void GridMap::setMapWidth(int width)
{
	this->m_iNumTilesWidth = width;
}

int GridMap::getMapWidth()
{
	return this->m_iNumTilesWidth;
}

void GridMap::setMapHeight(int height)
{
	this->m_iNumTilesHeight = height;
}

int GridMap::getMapHeight()
{
	return this->m_iNumTilesHeight;
}

void GridMap::ResetData()
{
	backgroundData.clear();
	backgroundData.~vector();
	foregroundData.clear();
	foregroundData.~vector();
	collisionData.clear();
	collisionData.~vector();

	for (unsigned int i = 0; i < m_cGridMap.size(); i++)
	{
		for (unsigned int j = 0; j < m_cGridMap[i].size(); j++)
		{
			delete m_cGridMap[i][j];
		}
	}
	m_cGridMap.clear();
	m_cGridMap.~vector();
}