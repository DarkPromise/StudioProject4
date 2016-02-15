#include "AxisAlignedBoundingBox.h"

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(float maxX, float maxY, float maxZ,float minX, float minY, float minZ)
{
	this->Max.Set(maxX,maxY,maxZ);
	this->Min.Set(minX, minY, minZ);
}

BoundingBox::~BoundingBox() {}

void BoundingBox::setMaxMin(Vector3 &Max, Vector3 &Min)
{
	this->Max.x = Max.x;
	this->Max.y = Max.y;
	this->Max.z = Max.z;

	this->Min.x = Min.x;
	this->Min.y = Min.y;
	this->Min.z = Min.z;
}

Vector3 BoundingBox::getClosestPoint(Vector3 Point)
{
	Vector3 min = this->Min;
	Vector3 max = this->Max;
	Vector3 result;

	float v = 0;
	v = Point.x;
	v = Math::Max(v, min.x);
	v = Math::Min(v, max.x);
	result.x = v;

	v = Point.y;
	v = Math::Max(v, min.y);
	v = Math::Min(v, max.y);
	result.y = v;

	v = Point.z;
	v = Math::Max(v, min.z);
	v = Math::Min(v, max.z);
	result.z = v;

	return result;
}