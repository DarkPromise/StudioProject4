#include "Component.h"
#include "Entity.h"
#include "LuaReader.h"

Component::~Component()
{

}

void Component::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
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