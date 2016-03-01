#ifndef ENTITY_GRID_OBJECT_H
#define ENTITY_GRID_OBJECT_H

#include "Entity.h"

class EntityGridObject : public Entity
{
public:
	enum OBJECT_TYPE
	{
		OBJECT_BOX,
		OBJECT_KEY,
		OBJECT_SWITCH,
		OBJECT_DOOR,

		OBJECT_UNDEFINED,
	};

	EntityGridObject();
	EntityGridObject(OBJECT_TYPE type);
	virtual ~EntityGridObject();

	virtual void Update(double dt);

public:
	void setObjectType(OBJECT_TYPE type);
	OBJECT_TYPE getObjectType();
	void addChildren(int y, int x, OBJECT_TYPE childType, GridMap * currMap);
	std::vector<EntityGridObject*> getChildren();
	void toggleObject(GridMap * currMap);
	void setToggleable(bool status);
	bool hasToggleAbility();

	void setIndexX(int x) { this->m_iIndexX = x; }
	void setIndexY(int y) { this->m_iIndexY = y; }
	void setActive(bool active);
	int get_m_iIndexX();
	int get_m_iIndexY();
	bool getActive();
private:
	OBJECT_TYPE m_eObjectType;
	std::vector<EntityGridObject*> m_cChildren;
	bool m_bToggleable;
	bool m_bToggled;
	int m_iIndexX;
	int m_iIndexY;
	bool active;
};

#endif