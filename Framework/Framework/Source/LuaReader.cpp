#include "LuaReader.h"

#include "EntityPlayer.h"
#include "EntityNPC.h"

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
		R"(function getKeys(name)
		s = {}
		for k, v in pairs(name)
		do
		table.insert(s,k)
		end
		return s
		end)";

	// Execute the code
	luaL_dostring(L, functionCode.c_str());
	lua_pcall(L, 0, 0, 0);

	// Get Keys
	lua_getglobal(L, "getKeys"); 
	lua_gettostack(tableName);

	// Execute Function
	lua_pcall(L, 1, 1, 0);

	lua_pushnil(L);

	std::vector<std::string> resultingStrings;

	while (lua_next(L, -2))
	{
		if (lua_type(L, -1) == LUA_TSTRING)
		{
			resultingStrings.push_back(lua_tostring(L, -1));
		}
		lua_pop(L, 1);
	}

	lua_settop(L, 0);

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

Entity * LuaReader::createEntity(const std::string & entityType, Camera * camera, InputHandler * inputHandler, GridMap * gridMap)
{
	using namespace luabridge;	
	auto newEntity = new Entity();
	auto components = getTables(entityType);
	LuaRef componentTable = getGlobal(L, entityType.c_str());

	for (auto componentName : components)
	{
		if (componentName == "InformationComponent")
		{
			LuaRef infoTable = componentTable[componentName];
			auto * infoComponent = new InformationComponent();
			infoComponent->CreateComponent(infoTable,entityType,gridMap);
			newEntity->addComponent(infoComponent);
		}
		else if (componentName == "GraphicsComponent")
		{
			LuaRef graphicsTable = componentTable[componentName];
			auto * graphicsComponent = new GraphicsComponent();
			graphicsComponent->CreateComponent(graphicsTable,entityType);
			newEntity->addComponent(graphicsComponent);
		}
		else if (componentName == "CameraComponent")
		{
			LuaRef cameraTable = componentTable[componentName];
			auto * cameraComponent = new CameraComponent(camera);
			cameraComponent->CreateComponent(cameraTable,entityType);
			newEntity->addComponent(cameraComponent);
		}
		else if (componentName == "ControllerComponent")
		{
			LuaRef controllerTable = componentTable[componentName];
			auto * controllerComponent = new ControllerComponent(inputHandler);
			controllerComponent->CreateComponent(controllerTable,entityType);
			newEntity->addComponent(controllerComponent);
		}
		else if (componentName == "GameplayComponent")
		{
			LuaRef gameplayTable = componentTable[componentName];
			auto * gameplayComponent = new GameplayComponent();
			gameplayComponent->CreateComponent(gameplayTable,entityType);
			newEntity->addComponent(gameplayComponent);
		}
		else if (componentName == "AIComponent")
		{
			LuaRef aiTable = componentTable[componentName];
			auto * aiComponent = new AIComponent();
			aiComponent->CreateComponent(aiTable,entityType);
			newEntity->addComponent(aiComponent);
		}
		else if (componentName == "WaypointComponent")
		{
			LuaRef waypointTable = componentTable[componentName];
			auto * waypointComponent = new WaypointComponent();
			waypointComponent->CreateComponent(waypointTable,entityType);
			newEntity->addComponent(waypointComponent);
		}
	}
	return newEntity;
}

void LuaReader::savePlayer(int playerIndexX, int playerIndexY, int level, bool m_bHasKey)
{
	ofstream file;
	file.open("Scripts//SavePlayer.lua");
	file << "SavePlayer = {" << std::endl;
	file << "playerGridX = \"" + std::to_string(playerIndexX) + "\"," << std::endl;
	file << "playerGridY = \"" + std::to_string(playerIndexY) + "\"," << std::endl;
	file << "level = \"" + std::to_string(level) + "\"," << std::endl;
	file << "hasKey = \"" + std::to_string(m_bHasKey) + "\"," << std::endl;
	file << "}" << std::endl;
	file.close();
}

void LuaReader::saveMap(std::vector<int> entityBoxesX, std::vector<int> entityBoxesY, std::vector<int> entityDoorsX, std::vector<int> entityDoorsY)
{
	ofstream file;
	file.open("Scripts//SaveMap.lua");
	file << "SaveMap = {" << std::endl;
	for (int i = 0; i < entityBoxesX.size(); i++)
	{
		file << "entityY" + std::to_string(i + 1) + "=\"" + std::to_string(entityBoxesX[i]) + "\"," << std::endl;
	}
	for (int j = 0; j < entityBoxesY.size(); j++)
	{
		file << "entityX" + std::to_string(j + 1) + "=\"" + std::to_string(entityBoxesY[j]) + "\"," << std::endl;
	}
	for (int i = 0; i < entityDoorsX.size(); i++)
	{
		file << "entityDoorY" + std::to_string(i + 1) + "=\"" + std::to_string(entityDoorsX[i]) + "\"," << std::endl;
	}
	for (int j = 0; j < entityDoorsY.size(); j++)
	{
		file << "entityDoorX" + std::to_string(j + 1) + "=\"" + std::to_string(entityDoorsY[j]) + "\"," << std::endl;
	}
	file << "}" << std::endl;
	file.close();
}