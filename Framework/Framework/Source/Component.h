#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>

class Entity;

class Component
{
public:
	virtual ~Component();

	virtual void Update(double dt);

	void setParent(Entity*);
	Entity* getParent();
private:
	Entity * m_cParent;
};

#endif