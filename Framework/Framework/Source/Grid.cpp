#include "Grid.h"

Grid::Grid(int tileSize)
: m_iTileSize(tileSize)
{
	m_cGridAABB.setMaxMin(Vector3((float)tileSize * 0.5f, (float)tileSize * 0.5f, 0.f), Vector3((float)-tileSize * 0.5f, (float)-tileSize * 0.5f, 0.f));
	m_cGridTiles.push_back(MeshBuilder::GenerateBoundingBox("GridBB", m_cGridAABB.Max, m_cGridAABB.Min,Color(0.f,0.f,1.f)));
}

Grid::~Grid()
{
	if (!m_cGridTiles.empty())
	{
		for (int i = 0; i < m_cGridTiles.size(); i++)
		{
			delete m_cGridTiles[i];
		}
	}
	m_cGridTiles.clear();
	m_cGridTiles.~vector();
}

void Grid::addTile(Mesh * mesh)
{
	m_cGridTiles.push_back(mesh);
}

Mesh * Grid::getMesh(int index)
{
	if (m_cGridTiles[index] != nullptr)
	{
		return m_cGridTiles[index];
	}
	return nullptr;
}

void Grid::setTileSize(int tileSize)
{
	this->m_iTileSize = tileSize;
}

int Grid::getTileSize()
{
	return this->m_iTileSize;
}