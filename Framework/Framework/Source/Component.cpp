#include "Component.h"
#include "Entity.h"

Component::~Component()
{

}

void Component::Update(double dt)
{

}

void Component::setParent(Entity* parent)
{
	this->m_cParent = parent;
}

Entity* Component::getParent()
{
	return this->m_cParent;
}