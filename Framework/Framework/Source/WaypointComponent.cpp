#include "WaypointComponent.h"
#include "LuaReader.h"

WaypointComponent::WaypointComponent()
: m_iWaypointIndex(0)
, m_iNextWaypointIndex(m_iWaypointIndex+1)
{

}

WaypointComponent::~WaypointComponent()
{
	if (!m_iWaypoints.empty())
	{
		for (int i = 0; i < m_iWaypoints.size(); i++)
		{
			delete m_iWaypoints[i];
		}
	}
	m_iWaypoints.clear();
	m_iWaypoints.~vector();
}

void WaypointComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name, GridMap * gridMap)
{
	auto waypointSize = tableInfo["numberOfPoints"];
	if (waypointSize.isNumber())
	{
		for (int i = 1; i <= waypointSize.cast<int>(); i++)
		{
			std::ostringstream ss1;
			ss1 << i;
			auto point = tableInfo["point" + ss1.str()];
			if (point.isTable())
			{
				if (point.length() == 2)
				{
					Waypoint * newPoint = new Waypoint();
					newPoint->indexX = point.rawget<int>(2);
					newPoint->indexY = point.rawget<int>(1);
					newPoint->position = gridMap->getGridMap()[point.rawget<int>(1)][point.rawget<int>(2)]->getGridPos();
					this->addWaypoint(newPoint);
				}
				else
				{
					std::cout << "WaypointComponent.point" + ss1.str() + " for " + name + " is not an array of 2 values!";
				}
			}
			else
			{
				std::cout << "WaypointComponent.point" + ss1.str() + " for " + name + " is not an array of 2 values!";
			}
		}
	}
	else
	{
		std::cout << "WaypointComponent.numberOfPoints for " + name + " is not a number!";
	}
}

void WaypointComponent::Update(double dt)
{

}

void WaypointComponent::setWaypointIndex(int index)
{
	this->m_iWaypointIndex = index;
}

int WaypointComponent::getWaypointIndex()
{
	return this->m_iWaypointIndex;
}

void WaypointComponent::setNextWaypointIndex(int nextIndex)
{
	this->m_iNextWaypointIndex = nextIndex;
}

int WaypointComponent::getNextWaypointIndex()
{
	return this->m_iNextWaypointIndex;
}

void WaypointComponent::setWaypoints(std::vector<Waypoint*> waypointList)
{
	this->m_iWaypoints = waypointList;
}

void WaypointComponent::addWaypoint(Waypoint * waypoint)
{
	this->m_iWaypoints.push_back(waypoint);
}

std::vector<Waypoint*> WaypointComponent::getWaypoints()
{
	return this->m_iWaypoints;
}