#include "GridMap.h"

GridMap::GridMap()
{

}

GridMap::~GridMap()
{
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
	Grid * newGrid;
	
	// Set the size of the vectors
	m_cGridMap.resize(xSize);
	for (unsigned int i = 0; i < xSize; i++)
	{
		m_cGridMap[i].resize(ySize);
	}

	// Populate the vector array with empty Grids
	for (unsigned int i = 0; i < xSize; i++)
	{
		for (unsigned int j = 0; j < ySize; j++)
		{
			newGrid = new Grid(tileSize);
			m_cGridMap[i][j] = newGrid;
		}
	}
}

void GridMap::RenderGrids(View * theView, bool renderBB)
{
	for (unsigned int i = 0; i < m_cGridMap.size(); i++)
	{
		for (unsigned int j = 0; j < m_cGridMap[i].size(); j++)
		{
			theView->modelStack.PushMatrix();
			theView->modelStack.Translate(i * m_cGridMap[i][j]->getTileSize(), j * m_cGridMap[i][j]->getTileSize(), 0);
			theView->RenderMesh(m_cGridMap[i][j]->getMesh(), false, false);
			theView->modelStack.PopMatrix();
		}
	}
}