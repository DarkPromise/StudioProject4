#include "Grid.h"
#include "LoadTGA.h"

Grid::Grid(int tileSize)
: m_iTileSize(tileSize)
, m_iTileID(-1)
, m_v3GridPos(Vector3(0.f,0.f,0.f))
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

void Grid::addTile(int tileID)
{
	Mesh * newMesh;
	this->m_iTileID = tileID;
	switch (tileID)
	{
	case 0:
		newMesh = MeshBuilder::GenerateQuad("Tile 0", Color(1.f, 1.f, 1.f), m_iTileSize);
		newMesh->textureArray[0] = LoadTGA("Images//Tiles//tile12.tga");
		//newMesh = MeshBuilder::GenerateBoundingBox("Test", Vector3(0.5f, 0.5f, 0), Vector3(-0.5f, -0.5f, 0), Color(0.f, 0.f, 1.f));
		m_cGridTiles.push_back(newMesh);
		break;
	case 1:
		newMesh = MeshBuilder::GenerateQuad("Tile 1", Color(1.f, 1.f, 1.f), m_iTileSize);
		newMesh->textureArray[0] = LoadTGA("Images//Tiles//tile2.tga");
		//newMesh = MeshBuilder::GenerateBoundingBox("Test", Vector3(0.5f, 0.5f, 0), Vector3(-0.5f, -0.5f, 0), Color(0.f, 0.f, 1.f));
		m_cGridTiles.push_back(newMesh);
		break;
	case 2:
		break;
	case 3:
		break;
	}
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

std::vector<Mesh*> Grid::getGridTiles()
{
	return this->m_cGridTiles;
}

void Grid::setTileID(int id)
{
	this->m_iTileID = id;
}

int Grid::getTileID()
{
	return this->m_iTileID;
}

void Grid::setGridPos(Vector3 gridPos)
{
	this->m_v3GridPos = gridPos;
}

Vector3 Grid::getGridPos()
{
	return this->m_v3GridPos;
}