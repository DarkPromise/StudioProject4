#include "GraphicsComponent.h"
#include "LuaReader.h"

GraphicsComponent::GraphicsComponent()
: m_meshList(NULL)
, m_iResLevel(0)
{
	
}

GraphicsComponent::~GraphicsComponent()
{
	if (!m_meshList.empty())
	{
		for (int i = 0; i < m_meshList.size(); i++)
		{
			delete m_meshList[i];
		}
	}
	m_meshList.clear();
	m_meshList.~vector();
}

void GraphicsComponent::CreateComponent(luabridge::LuaRef& tableInfo)
{
	using namespace luabridge;
	auto meshType = tableInfo["meshType"];
	auto meshName = tableInfo["meshName"];

	std::string name = "undefined";
	if (meshName.isString())
	{
		name = meshName.cast<std::string>();
	}
	else
	{
		std::cout << "GraphicsComponent.meshName is not a string!" << std::endl;
	}

	auto meshImage = tableInfo["meshImage"];
	std::string imagePath = "Images//Tiles//unknown.tga";
	if (meshImage.isString())
	{
		imagePath = meshImage.cast<std::string>();
	}
	else
	{
		std::cout << "GraphicsComponent.meshImage is not a string!" << std::endl;
	}

	auto meshSize = tableInfo["meshSize"];
	float size = 1.f;
	if (meshSize.isNumber())
	{
		size = meshSize.cast<float>();
	}
	else
	{
		std::cout << "GraphicsComponent.meshSize is not a number!" << std::endl;
	}

	auto meshColor = tableInfo["meshColor"];
	Color color = Color(0.f, 0.f, 0.f);
	if (meshColor.isTable())
	{
		if (meshColor.length() == 3)
		{
			color.Set(meshColor.rawget<float>(1), meshColor.rawget<float>(2), meshColor.rawget<float>(3));
		}
		else
		{
			std::cout << "GraphicsComponent.meshColor is not an array with 3 values!" << std::endl;
		}
	}

	auto meshAlpha = tableInfo["meshAlpha"];
	float alpha = 1.f;
	if (meshAlpha.isNumber())
	{
		alpha = meshAlpha.cast<float>();
	}
	else
	{
		std::cout << "GraphicsComponent.meshAlpha is not a number!" << std::endl;
	}

	if (meshType.isString())
	{
		if (meshType.cast<std::string>() == "Quad")
		{
			addMesh(MeshBuilder::GenerateQuad(name, color, size));
			getMesh()->textureArray[0] = LoadTGA(imagePath.c_str());
			getMesh()->alpha = alpha;
		}
		else if (meshType.cast<std::string>() == "AABB")
		{
			addMesh(MeshBuilder::GenerateBoundingBox(name, Vector3(size*0.5f, size*0.5f, size*0.5f), Vector3(-size*0.5f, -size*0.5f, -size*0.5f),color));
		}
		else if (meshType.cast<std::string>() == "2DAnimation")
		{
			// TODO ADD SPRITE ANIMATION
		}
		else
		{ 
			std::cout << "GraphicsComponent.meshType is undefined" << std::endl;
		}
	}
	else
	{
		std::cout << "GraphicsComponent.meshType is not a string!" << std::endl;
	}
}

void GraphicsComponent::Update(double dt)
{

}

void GraphicsComponent::addMesh(Mesh * mesh)
{
	this->m_meshList.push_back(mesh);
}

Mesh * GraphicsComponent::getMesh(int index)
{
	return this->m_meshList[index];
}

void GraphicsComponent::setResLevel(int i)
{
	this->m_iResLevel = i;
}

int GraphicsComponent::getResLevel()
{
	return this->m_iResLevel;
}