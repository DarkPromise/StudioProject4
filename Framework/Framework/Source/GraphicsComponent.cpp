#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent()
: m_meshList(NULL)
, m_iResLevel(0)
{
	
}

GraphicsComponent::~GraphicsComponent()
{
	if (!m_meshList.empty())
	{
		for (int i = 0; i < m_meshList.size(); i++)
		{
			delete m_meshList[i];
		}
	}
	m_meshList.clear();
	m_meshList.~vector();
}

void GraphicsComponent::Update(double dt)
{

}

void GraphicsComponent::addMesh(Mesh * mesh)
{
	this->m_meshList.push_back(mesh);
}

Mesh * GraphicsComponent::getMesh(int index)
{
	return this->m_meshList[index];
}

void GraphicsComponent::setResLevel(int i)
{
	this->m_iResLevel = i;
}

int GraphicsComponent::getResLevel()
{
	return this->m_iResLevel;
}