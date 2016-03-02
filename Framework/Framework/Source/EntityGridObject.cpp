#include "EntityGridObject.h"
#include "SoundManager.h"

EntityGridObject::EntityGridObject()
: m_eObjectType(OBJECT_UNDEFINED)
, m_cChildren(NULL)
, m_bToggleable(false)
, m_bToggled(false)
, m_iIndexX(-1)
, m_iIndexY(-1)
, active(true)
{

}

EntityGridObject::EntityGridObject(OBJECT_TYPE type)
: m_eObjectType(type)
, active(true)
{
	switch (m_eObjectType)
	{
	case OBJECT_BOX:
		break;
	case OBJECT_KEY:
		break;
	case OBJECT_SWITCH:
		m_bToggleable = true;
		break;
	case OBJECT_DOOR:
		m_bToggleable = true;
		break;
	}
}

EntityGridObject::~EntityGridObject()
{
	//if (!m_cChildren.empty())
	//{
	//	for (int i = 0; i < m_cChildren.size(); i++)
	//	{
	//		delete m_cChildren[i];
	//	}
	//}
	m_cChildren.clear();
	m_cChildren.~vector();
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

void EntityGridObject::addChildren(int y, int x, OBJECT_TYPE childType, GridMap * currMap)
{
	EntityGridObject * newChild = new EntityGridObject(childType);
	newChild->m_iIndexX = x;
	newChild->m_iIndexY = y;

	auto childGC = new GraphicsComponent();
	switch (childType)
	{
		case OBJECT_UNDEFINED:
			currMap->getGridMap()[y][x]->setremoveDoor(true);
			currMap->getGridMap()[y][x]->replaceTile(Grid::TILE_FLOOR, BACKGROUND_TILE);
		break;
		
		case OBJECT_BOX:
			break;
		
		case OBJECT_KEY:
			break;
		
		case OBJECT_SWITCH:
			break;
		
		case OBJECT_DOOR:
			newChild->setToggleable(true);
			childGC->addMesh(MeshBuilder::GenerateQuad("Door", Color(1.f, 0.f, 0.f), 32.f));
			childGC->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile109.tga");
			newChild->addComponent(childGC);
			currMap->getGridMap()[y][x]->addGridEntity(newChild);
		break;
	}
	this->m_cChildren.push_back(newChild);
}

std::vector<EntityGridObject*> EntityGridObject::getChildren()
{
	return this->m_cChildren;
}

void EntityGridObject::toggleObject(GridMap * currMap)
{
	if (!m_bToggled)
	{
		m_bToggled = true;
	}
	else
	{
		m_bToggled = false;
	}

	auto graphicsComponent = this->getComponent<GraphicsComponent>();
	Mesh * newMesh;

	switch (this->m_eObjectType)
	{
	case OBJECT_BOX:
		break;
	case OBJECT_KEY:
		break;
	case OBJECT_SWITCH:
		delete graphicsComponent->getMesh();
		graphicsComponent->setMesh(MeshBuilder::GenerateQuad("Activated", Color(0.f, 0.f, 0.f), 16.f));
		graphicsComponent->getMesh()->textureArray[0] = LoadTGA("Images//Tiles//tile_switch_activate.tga");
		graphicsComponent->getMesh()->alpha = 1.f;
		break;
	case OBJECT_DOOR:
		SoundManager::playSound("Sounds//lever.wav", false);
		SoundManager::playSound("Sounds//opendoor.wav", false);
		currMap->getGridMap()[this->m_iIndexY][this->m_iIndexX]->setremoveDoor(true);
		currMap->getGridMap()[this->m_iIndexY][this->m_iIndexX]->replaceTile(Grid::TILE_FLOOR,BACKGROUND_TILE);
		currMap->getGridMap()[this->m_iIndexY][this->m_iIndexX]->deleteEntity();
		break;
	}
}

void EntityGridObject::setToggleable(bool status)
{
	this->m_bToggleable = status;
}

bool EntityGridObject::hasToggleAbility()
{
	return this->m_bToggleable;
}

int EntityGridObject::get_m_iIndexX()
{
	return m_iIndexX;
}

int EntityGridObject::get_m_iIndexY()
{
	return m_iIndexY;
}

void EntityGridObject::setActive(bool active)
{
	this->active = active;
}

bool EntityGridObject::getActive()
{
	return active;
}
