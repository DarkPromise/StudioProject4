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

Entity * LuaReader::createEntity(const std::string & entityType, Camera * camera, InputHandler * inputHandler)
{
	using namespace luabridge;
	
	auto newEntity = new Entity();
	if (entityType == "Player")
	{

	}
	else if (entityType == "NPC")
	{

	}

	auto components = getTables(entityType);
	LuaRef componentTable = getGlobal(L, entityType.c_str());

	for (auto componentName : components)
	{
		if (componentName == "InformationComponent")
		{
			LuaRef infoTable = componentTable[componentName];
			auto * infoComponent = new InformationComponent();
			infoComponent->CreateComponent(infoTable);
			newEntity->addComponent(infoComponent);
		}
		else if (componentName == "GraphicsComponent")
		{
			LuaRef graphicsTable = componentTable[componentName];
			auto * graphicsComponent = new GraphicsComponent();
			graphicsComponent->CreateComponent(graphicsTable);
			newEntity->addComponent(graphicsComponent);
		}
		else if (componentName == "CameraComponent")
		{
			LuaRef cameraTable = componentTable[componentName];
			auto * cameraComponent = new CameraComponent(camera);
			cameraComponent->CreateComponent(cameraTable);
			newEntity->addComponent(cameraComponent);
		}
		else if (componentName == "ControllerComponent")
		{
			LuaRef controllerTable = componentTable[componentName];
			auto * controllerComponent = new ControllerComponent(inputHandler);
			controllerComponent->CreateComponent(controllerTable);
			newEntity->addComponent(controllerComponent);
		}
		else if (componentName == "GameplayComponent")
		{
			LuaRef gameplayTable = componentTable[componentName];
			auto * gameplayComponent = new GameplayComponent();
			gameplayComponent->CreateComponent(gameplayTable);
			newEntity->addComponent(gameplayComponent);
		}
	}
	return newEntity;
}

void LuaReader::saveFile(int playerIndexX, int playerIndexY, int level)
{
	ofstream file;
	file.open("Scripts//Save.lua");
	file << "Save = {" << std::endl;
	file << "playerGridX = \"" + std::to_string(playerIndexX) + "\"," << std::endl;
	file << "playerGridY = \"" + std::to_string(playerIndexY) + "\"," << std::endl;
	file << "level = \"" + std::to_string(level) + "\"," << std::endl;
	file << "}" << std::endl;
	file.close();
}