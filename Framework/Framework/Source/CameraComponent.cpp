#include "CameraComponent.h"
#include "Entity.h"
#include "LuaReader.h"

CameraComponent::CameraComponent()
: m_Camera(NULL)
, m_v3CamDir(Vector3(0.f, 0.f, 0.f))
, m_v3CamOffset(Vector3(0.f, 0.f, 0.f))
{

}

CameraComponent::CameraComponent(Camera * camera)
{
	this->m_Camera = camera;
}

CameraComponent::~CameraComponent()
{
	
}

void CameraComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{
	auto cameraOffset = tableInfo["cameraOffset"];
	if (cameraOffset.isTable())
	{
		if (cameraOffset.length() == 3)
		{
			// LuaRef starts indexes from 1, not 0
			this->m_v3CamOffset = Vector3(cameraOffset.rawget<float>(1), cameraOffset.rawget<float>(2), cameraOffset.rawget<float>(3));
		}
		else
		{
			std::cout << "CameraComponent.cameraOffset for " + name + " is not an array with 3 values!" << std::endl;
		}
	}
	else
	{
		std::cout << "CameraComponent.cameraOffset for " + name + " is not an array with 3 values!" << std::endl;
	}

	auto cameraMode = tableInfo["cameraMode"];
	if (cameraMode.isNumber())
	{
		this->m_Camera->setCameraMode(Camera::CAMERA_MODE(cameraMode.cast<int>()));
	}
	else
	{
		std::cout << "CameraComponent.cameraMode for " + name + " is not a valid mode!" << std::endl;
	}
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
		if (infoC)
		{
			Vector3 newCamPos = infoC->getPosition() + m_v3CamOffset;
			m_Camera->setCameraPos(newCamPos);
			Vector3 newCamTarget = infoC->getPosition();
			m_Camera->setCameraTarget(newCamTarget);
		}
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

void CameraComponent::setCameraOffset(Vector3 offset)
{
	this->m_v3CamOffset = offset;
}

Vector3 CameraComponent::getCameraOffset()
{
	return this->m_v3CamOffset;
}