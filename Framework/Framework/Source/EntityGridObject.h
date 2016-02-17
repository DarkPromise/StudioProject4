#ifndef ENTITY_GRID_OBJECT_H
#define ENTITY_GRID_OBJECT_H

#include "Entity.h"

class EntityGridObject : public Entity
{
public:
	enum OBJECT_TYPE
	{
		OBJECT_BOX = 0,
		OBJECT_KEY,
		OBJECT_UNDEFINED,
	};
	EntityGridObject();
	EntityGridObject(OBJECT_TYPE type);
	virtual ~EntityGridObject();

	virtual void Update(double dt);
public:
	void setObjectType(OBJECT_TYPE type);
	OBJECT_TYPE getObjectType();
private:
	OBJECT_TYPE m_eObjectType;
};

#endif