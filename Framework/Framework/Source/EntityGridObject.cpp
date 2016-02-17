#include "EntityGridObject.h"

EntityGridObject::EntityGridObject()
: m_eObjectType(OBJECT_UNDEFINED)
{

}

EntityGridObject::EntityGridObject(OBJECT_TYPE type)
: m_eObjectType(type)
{

}

EntityGridObject::~EntityGridObject()
{

}

void EntityGridObject::Update(double dt)
{

}

void EntityGridObject::setObjectType(OBJECT_TYPE type)
{
	this->m_eObjectType = type;
}

EntityGridObject::OBJECT_TYPE EntityGridObject::getObjectType()
{
	return this->m_eObjectType;
}
