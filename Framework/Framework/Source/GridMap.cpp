#include "GridMap.h"

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

	if (!m_cTileMap.empty())
	{
		for (int i = 0; i < m_cTileMap.size(); i++)
		{
			delete m_cTileMap[i];
		}
	}
	m_cTileMap.clear();
	m_cTileMap.~vector();

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

void GridMap::Init(int xSize, int ySize, int tileSize)
{
	m_iNumTilesWidth = xSize;
	m_iNumTilesHeight = ySize;
	m_iTileSize = tileSize;

	Grid * newGrid;

	// Set the size of the vectors
	m_cGridMap.resize(xSize);
	for (unsigned int i = 0; i < xSize; i++)
	{
		m_cGridMap[i].resize(ySize);
	}

	// Populate the vector array with empty Grids
	for (unsigned int i = 0; i < xSize; i++) // - Axis
	{
		for (unsigned int j = 0; j < ySize; j++)
		{
			newGrid = new Grid(tileSize);
			newGrid->setGridPos(Vector3(i * tileSize, j * tileSize, 0.f));
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

void GridMap::RenderGrids(View * theView, bool renderBB)
{
	for (unsigned int i = 0; i < m_cGridMap.size(); i++)
	{
		for (unsigned int j = 0; j < m_cGridMap[i].size(); j++)
		{
			if (renderBB)
			{
				theView->modelStack.PushMatrix();
				theView->modelStack.Translate(m_cGridMap[i][j]->getGridPos().x, m_cGridMap[i][j]->getGridPos().y, 0);
				theView->RenderMesh(m_cGridMap[i][j]->getMesh(), false, false);
				theView->modelStack.PopMatrix();
			}
			if (m_cGridMap[i][j]->getGridTiles().size() > 1)
			{
				theView->modelStack.PushMatrix();
				theView->modelStack.Translate(m_cGridMap[i][j]->getGridPos().x, m_cGridMap[i][j]->getGridPos().y, 0);
				theView->RenderMesh(m_cGridMap[i][j]->getMesh(1), false, false);
				theView->modelStack.PopMatrix();
			}
		}
	}
}

std::vector<std::vector<Grid*>> GridMap::getGridMap()
{
	return this->m_cGridMap;
}

std::vector<Mesh*> GridMap::getTileMap()
{
	return this->m_cTileMap;
}

bool GridMap::LoadData(std::string backgroundCSV, std::string foregroundCSV)
{
	if (LoadForegroundMap(foregroundCSV))
	{
		std::cout << foregroundCSV << " has been loaded. " << std::endl;
		if (LoadBackgroundMap(backgroundCSV))
		{
			std::cout << backgroundCSV << " has been loaded. " << std::endl << std::endl;
			return true;
		}
		std::cout << "Failed to load background" << std::endl;
		return false;
	}
	std::cout << "Failed to load foreground" << std::endl;
	return false;
}

bool GridMap::LoadBackgroundMap(const std::string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;

	std::ifstream file(mapName.c_str());
	if (file.is_open())
	{
		int i = 0, k = 0;
		while (file.good())
		{
			std::string aLineOfText = "";
			getline(file, aLineOfText);

			if (theLineCounter >= m_iNumTilesHeight)
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
					while (getline(iss, token, ',') && (theColumnCounter<m_iNumTilesWidth))
					{
						backgroundData[theLineCounter][theColumnCounter++] = atoi(token.c_str());
					}
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
						foregroundData[theLineCounter][theColumnCounter++] = atoi(token.c_str());
					}
				}
			}

			theLineCounter++;
		}
	}
	return true;
}