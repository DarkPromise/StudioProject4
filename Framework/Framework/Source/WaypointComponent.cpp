#include "WaypointComponent.h"

WaypointComponent::WaypointComponent()
: m_iWaypointIndex(0)
, m_iTempWaypointIndex(0)
{

}

WaypointComponent::~WaypointComponent()
{

}

void WaypointComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{

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

void WaypointComponent::setTempWaypointIndex(int tempIndex)
{
	this->m_iTempWaypointIndex = tempIndex;
}

int WaypointComponent::getTempWaypointIndex()
{
	return this->m_iTempWaypointIndex;
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