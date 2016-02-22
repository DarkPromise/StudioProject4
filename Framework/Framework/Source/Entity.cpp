#include "Entity.h"

#include "Component.h"

Entity::Entity()
{

}

Entity::~Entity()
{
	if (!m_componentList.empty())
	{
		for (int i = 0; i < m_componentList.size(); i++)
		{
			delete m_componentList[i];
		}
	}
	m_componentList.clear();
	m_componentList.~vector();
}

void Entity::Update(double dt)
{

}

void Entity::addComponent(Component * component)
{
	if (component->getParent() == NULL) //Component has no parent
	{
		component->setParent(this);
	}
	m_componentList.push_back(component);
}