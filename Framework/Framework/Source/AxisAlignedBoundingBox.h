/******************************************************************************/
/*!
\file	BoundingBox.h
\author Josh
\brief
Header file for BoundingBox
*/
/******************************************************************************/
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

//Josh Code
#include "Vector3.h"
#include "glew.h"

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(float,float,float,float,float,float);
	~BoundingBox();

	int id;

	float collisionRadius;

	Vector3 Max;
	Vector3 Min;

	virtual int getID(){ return id; }
	virtual float getCollisionRadius(){ return collisionRadius; }
	void setMaxMin(Vector3 &Max,Vector3 &Min);
	Vector3 getClosestPoint(Vector3 point);
};

#endif