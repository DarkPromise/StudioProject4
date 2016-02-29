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

void GraphicsComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{
	using namespace luabridge;
	auto meshType = tableInfo["meshType"];
	auto meshName = tableInfo["meshName"];

	std::string meshname = "undefined";
	if (meshName.isString())
	{
		meshname = meshName.cast<std::string>();
	}
	else
	{
		std::cout << "GraphicsComponent.meshName for " + name + " is not a string!" << std::endl;
	}

	auto meshSize = tableInfo["meshSize"];
	float size = 1.f;
	if (meshSize.isNumber())
	{
		size = meshSize.cast<float>();
	}
	else
	{
		std::cout << "GraphicsComponent.meshSize for " + name + " is not a number!" << std::endl;
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
			std::cout << "GraphicsComponent.meshColor for " + name + " is not an array with 3 values!" << std::endl;
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
		std::cout << "GraphicsComponent.meshAlpha for " + name + " is not a number!" << std::endl;
	}

	if (meshType.isString())
	{
		if (meshType.cast<std::string>() == "Quad")
		{
			auto numberofimages = tableInfo["meshNumberOfImages"];
			if (numberofimages.isNumber())
			{
				if (numberofimages.cast<int>() > 1)
				{
					for (int i = 1; i <= numberofimages.cast<int>(); i++)
					{
						std::ostringstream ss1;
						ss1 << i;
						auto meshImage = tableInfo["meshImage" + ss1.str()];
						std::string imagePath = "Images//Tiles//unknown.tga";
						if (meshImage.isString())
						{
							imagePath = meshImage.cast<std::string>();
						}
						else
						{
							std::cout << "GraphicsComponent.meshImage" << ss1.str() << " for " + name + " is not a string!" << std::endl;
						}
						std::cout << imagePath.c_str() << std::endl;
						addMesh(MeshBuilder::GenerateQuad(name, color, size));
						getMesh(i-1)->textureArray[0] = LoadTGA(imagePath.c_str());
						getMesh(i-1)->alpha = alpha;
					}
				}
			}
			else
			{
				std::cout << "GraphicsComponent.meshNumberOfImages for " + name + " is not a number!" << std::endl;

				auto meshImage = tableInfo["meshImage"];
				std::string imagePath = "Images//Tiles//unknown.tga";
				if (meshImage.isString())
				{
					imagePath = meshImage.cast<std::string>();
				}
				else
				{
					std::cout << "GraphicsComponent.meshImage for " + name + " is not a string!" << std::endl;
				}
				addMesh(MeshBuilder::GenerateQuad(name, color, size));
				getMesh()->textureArray[0] = LoadTGA(imagePath.c_str());
				getMesh()->alpha = alpha;
			}
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
			std::cout << "GraphicsComponent.meshType for " + name + " is undefined" << std::endl;
		}
	}
	else
	{
		std::cout << "GraphicsComponent.meshType for " + name + " is not a string!" << std::endl;
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

void GraphicsComponent::setMesh(Mesh * mesh, int index)
{
	this->m_meshList[index] = mesh;
}