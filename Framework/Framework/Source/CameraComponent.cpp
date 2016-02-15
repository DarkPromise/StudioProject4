#include "CameraComponent.h"
#include "Entity.h"

CameraComponent::CameraComponent()
: m_Camera(nullptr)
{

}

CameraComponent::CameraComponent(Camera * camera)
{
	this->m_Camera = camera;
}

CameraComponent::~CameraComponent()
{
	
}

void CameraComponent::Update(double dt)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();

	switch (m_Camera->getCameraMode())
	{
	case Camera::CM_THIRD_PERSON_FIXED_OFFSET:
		if (infoC)
		{
			LookAround(dt);

			Vector3 newCamPos = infoC->getPosition() - infoC->getDirection().Normalized() * 30.f;
			newCamPos.y = infoC->getPosition().y + 40.f;
			m_Camera->setCameraPos(newCamPos);
			Vector3 newCamTarget = infoC->getPosition() + infoC->getDirection().Normalized() * 40.f;
			m_Camera->setCameraTarget(newCamTarget);
		}

		break;
	case Camera::CM_THIRD_PERSON_FOLLOW_ENTITY:
		break;
	}
}

void CameraComponent::setCamera(Camera * camera)
{
	this->m_Camera = camera;
}

Camera * CameraComponent::getCamera()
{
	return this->m_Camera;
}

void CameraComponent::setCameraDirection(Vector3 direction)
{
	this->m_v3CamDir = direction;
}

Vector3 CameraComponent::getCameraDirection()
{
	return this->m_v3CamDir;
}

void CameraComponent::LookAround(double dt)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();

	if (infoC)
	{
		Mtx44 rotation;
		rotation.SetToRotation(m_Camera->getCameraYaw(),0, 1, 0);
		Vector3 curDirection = infoC->getDirection().Normalized();
		curDirection = rotation * curDirection;
		infoC->setDirection(curDirection);
	}
}