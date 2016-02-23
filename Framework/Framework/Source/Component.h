#ifndef COMPONENT_H
#define COMPONENT_H

#pragma warning(disable:4018)

#include <iostream>

namespace luabridge 
{
	class LuaRef;
}

class Entity;

class Component
{
public:
	virtual ~Component();

	virtual void CreateComponent(luabridge::LuaRef& tableInfo, std::string name);
	virtual void Update(double dt);

	void setParent(Entity*);
	Entity* getParent();
private:
	Entity * m_cParent;
};

#endif