#ifndef ENTITY_GRID_OBJECT_H
#define ENTITY_GRID_OBJECT_H

#include "Entity.h"

class EntityGridObject : public Entity
{
public:
	EntityGridObject();
	virtual ~EntityGridObject();

	virtual void Update(double dt);

private:
};

#endif