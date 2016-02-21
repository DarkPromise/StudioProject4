#ifndef ENTITY_TEST_H
#define ENTITY_TEST_H

#include "Entity.h"

class EntityTest : public Entity
{
public:
	EntityTest();
	virtual ~EntityTest();

	virtual void Update(double dt);

	bool m_bHasKey;
	bool m_levelClear;
	bool m_showkeyRequired;
	bool m_switchActivated;
	float m_showKeyRequiredTimer;
	float m_switchActivatedTimer;
private:
};

#endif