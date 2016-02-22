#ifndef ENTITY_TEST_H
#define ENTITY_TEST_H

#include "Entity.h"

class EntityTest : public Entity
{
public:
	EntityTest();
	virtual ~EntityTest();
	virtual void Update(double dt);

private:
};

#endif