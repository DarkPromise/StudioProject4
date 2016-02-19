#include "LuaReader.h"

#include "EntityPlayer.h"

LuaReader::LuaReader(const std::string & filepath)
{
	L = luaL_newstate();
	if (luaL_loadfile(L, filepath.c_str()) || lua_pcall(L, 0, 0, 0))
	{
		std::cout << "Failed to load script at " << filepath  << std::endl;
		L = 0;
	}
	if (L)
	{
		luaL_openlibs(L);
	}
}

LuaReader::~LuaReader()
{
	if (L)
	{
		lua_close(L);
	}
}

void LuaReader::printError(const std::string & variableName, const std::string reason)
{
	std::cout << "Could not find (" << variableName << ")" << std::endl;
	std::cout << "Reason : " << reason << std::endl;
}

std::vector<std::string> LuaReader::getTables(const std::string & tableName)
{
	// Using Lua to create a function using code 
	std::string functionCode =
		"function getKeys(name) "
		"s = \"\""
		"for k, v in pairs(_G[name]) do "
		"    s = s..k..\",\" "
		"    end "
		"return s "
		"end";

	// Execute the code
	luaL_loadstring(L, functionCode.c_str());
	lua_pcall(L, 0, 0, 0);

	// Get Keys
	lua_getglobal(L, "getKeys"); 
	lua_pushstring(L, tableName.c_str());

	// Execute Function
	lua_pcall(L, 1, 1, 0);

	std::string test = lua_tostring(L, -1);
	std::vector<std::string> resultingStrings;
	std::string temp = "";

	for (unsigned int i = 0; i < test.size(); i++) 
	{
		if (test.at(i) != ',') 
		{
			temp += test.at(i);
		}
		else 
		{
			resultingStrings.push_back(temp);
			temp = "";
		}
	}
	this->Clean();

	return resultingStrings;
}

bool LuaReader::lua_gettostack(const std::string & variableName)
{
	currLevel = 0;
	std::string var = "";
	for (unsigned int i = 0; i < variableName.size(); i++)
	{
		if (variableName.at(i) == '.')
		{
			if (currLevel == 0)
			{
				lua_getglobal(L, var.c_str());
			}
			else
			{
				lua_getfield(L, -1, var.c_str());
			}

			if (lua_isnil(L, -1))
			{
				printError(variableName, var + " is not defined");
				return false;
			}
			else
			{
				var = "";
				currLevel++;
			}
		}
		else
		{
			var += variableName.at(i);
		}
	}
	if (currLevel == 0)
	{
		lua_getglobal(L, var.c_str());
	}
	else
	{
		lua_getfield(L, -1, var.c_str());
	}
	if (lua_isnil(L, -1))
	{
		printError(variableName, var + " is not defined");
		return false;
	}
	return true;
}

Entity * LuaReader::createEntity(const std::string & entityType, Camera * camera)
{
	auto components = getTables(entityType);

	Entity * newEntity = NULL;

	if (entityType == "Player")
	{
		newEntity = new EntityPlayer();
	}
	else
	{
		// Make Other Entities
	}

	if (newEntity)
	{
		for (auto name : components)
		{
			if (name == "InformationComponent")
			{
				auto * infoComponent = new InformationComponent();
				infoComponent->setName(this->get<std::string>(entityType + "." + name + ".name"));
				infoComponent->setPosition(this->get<Vector3>(entityType + "." + name + ".position"));
				infoComponent->setPosition(this->get<Vector3>(entityType + "." + name + ".direction"));
				infoComponent->setSize(this->get<float>(entityType + "." + name + ".size"));
				infoComponent->setType(InformationComponent::ENTITY_TYPE(this->get<int>(entityType + "." + name + ".type")));
				newEntity->addComponent(infoComponent);
			}
			else if (name == "GraphicsComponent")
			{
				auto * graphicsComponent = new GraphicsComponent();
				if (this->get<std::string>(entityType + "." + name + ".meshType") == "Cube") // Can be used with Enums, **NOTE : This is just testing, there can be else/if for the meshTypes
				{
					graphicsComponent->addMesh(MeshBuilder::GenerateCube(entityType + " (LOW RES)", this->get<Color>(entityType + "." + name + ".color"), this->get<float>(entityType + "." + name + ".meshSize")));
					graphicsComponent->addMesh(MeshBuilder::GenerateCube(entityType + " (MED RES)", this->get<Color>(entityType + "." + name + ".color"), this->get<float>(entityType + "." + name + ".meshSize")));
					graphicsComponent->addMesh(MeshBuilder::GenerateCube(entityType + " (HIGH RES)", this->get<Color>(entityType + "." + name + ".color"), this->get<float>(entityType + "." + name + ".meshSize")));
				}
				newEntity->addComponent(graphicsComponent);
			}
			else if (name == "CameraComponent")
			{
				auto * cameraComponent = new CameraComponent(camera);
				cameraComponent->getCamera()->setCameraMode(Camera::CAMERA_MODE(this->get<int>(entityType + "." + name + ".mode")));
				newEntity->addComponent(cameraComponent);
			}
			else if (name == "CollisionComponent")
			{
				auto * collisionComponent = new CollisionComponent();
				collisionComponent->setMinMax(this->get<Vector3>(entityType + "." + name + ".min"), this->get<Vector3>(entityType + "." + name + ".max"));
				collisionComponent->setMesh(MeshBuilder::GenerateBoundingBox("PlayerBB", collisionComponent->getAABB()->Max, collisionComponent->getAABB()->Min, Color(0.f, 0.f, 1.f)));
				newEntity->addComponent(collisionComponent);
			}
			else if (name == "HealthComponent")
			{
				auto * healthComponent = new HealthComponent();
				healthComponent->setHealth(this->get<float>(entityType + "." + name + ".minHp"));
				healthComponent->setMaxHealth(this->get<float>(entityType + "." + name + ".maxHp"));
				newEntity->addComponent(healthComponent);
			}
		}
	}
	return newEntity;
}

void LuaReader::saveFile(int playerIndexX, int playerIndexY)
{
	ofstream file;
	file.open("Scripts//Save.lua");
	file << "Save = {" << std::endl;
	file << "playerGridX = \"" + std::to_string(playerIndexX) + "\"," << std::endl;
	file << "playerGridY = \"" + std::to_string(playerIndexY) + "\"," << std::endl;
	file << "}" << std::endl;
	file.close();
}