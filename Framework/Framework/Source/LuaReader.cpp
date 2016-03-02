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
			waypointComponent->CreateComponent(waypointTable,entityType, gridMap);
			newEntity->addComponent(waypointComponent);
		}
		else if (componentName == "HealthComponent")
		{
			LuaRef healthTable = componentTable[componentName];
			auto * healthComponent = new HealthComponent();
			healthComponent->CreateComponent(healthTable, entityType);
			newEntity->addComponent(healthComponent);
		}
	}
	return newEntity;
}

void LuaReader::savePlayer(int playerIndexX, int playerIndexY, int level, bool m_bHasKey, float timing, Vector3 direction, Vector3 rotation, int health)
{
	std::stringstream directionX, directionY, directionZ;
	directionX.precision(1); directionY.precision(1); directionZ.precision(1);
	directionX << direction.x; directionY << direction.y; directionZ << direction.z;
	std::stringstream rotationX, rotationY, rotationZ;
	rotationX.precision(3); rotationY.precision(3); rotationZ.precision(3);
	rotationX << rotation.x; rotationY << rotation.y; rotationZ << rotation.z;

	ofstream file;
	file.open("Scripts//SavePlayer.lua");
	file << "SavePlayer = {" << std::endl;
	file << "playerGridX = \"" + std::to_string(playerIndexX) + "\"," << std::endl;
	file << "playerGridY = \"" + std::to_string(playerIndexY) + "\"," << std::endl;
	file << "level = \"" + std::to_string(level) + "\"," << std::endl;
	file << "hasKey = \"" + std::to_string(m_bHasKey) + "\"," << std::endl;
	file << "timing = \"" + std::to_string(timing) + "\"," << std::endl;
	file << "direction = {" + directionX.str() + "," + directionY.str() + "," + directionZ.str() + "}," << std::endl;
	file << "rotation = {" + rotationX.str() + "," + rotationY.str() + "," + rotationZ.str() + "}," << std::endl;
	file << "health = \"" + std::to_string(health) + "\"," << std::endl;
	file << "}" << std::endl;
	file.close();
}

void LuaReader::saveBoxes(std::vector<int> entityBoxesX, std::vector<int> entityBoxesY, int totalBoxes)
{
	ofstream file;
	file.open("Scripts//SaveBoxes.lua");
	file << "SaveBoxes = {" << std::endl;
	for (int i = 0; i < entityBoxesX.size(); i++)
	{
		file << "entityY" + std::to_string(i + 1) + "=\"" + std::to_string(entityBoxesX[i]) + "\"," << std::endl;
	}
	for (int j = 0; j < entityBoxesY.size(); j++)
	{
		file << "entityX" + std::to_string(j + 1) + "=\"" + std::to_string(entityBoxesY[j]) + "\"," << std::endl;
	}
	file << "totalBoxes = \"" + std::to_string(totalBoxes) + "\"," << std::endl;
	file << "}" << std::endl;
	file.close();
}

void LuaReader::saveDoors(std::vector<int> entityDoorsX, std::vector<int> entityDoorsY, std::vector<int> entityDoorsOpenX, std::vector<int> entityDoorsOpenY, int totalCloseDoors, int totalOpenDoors)
{
	ofstream file;
	file.open("Scripts//SaveDoors.lua");
	file << "SaveDoors = {" << std::endl;
	for (int i = 0; i < entityDoorsX.size(); i++)
	{
		file << "entityDoorY" + std::to_string(i + 1) + "=\"" + std::to_string(entityDoorsX[i]) + "\"," << std::endl;
	}
	for (int j = 0; j < entityDoorsY.size(); j++)
	{
		file << "entityDoorX" + std::to_string(j + 1) + "=\"" + std::to_string(entityDoorsY[j]) + "\"," << std::endl;
	}
	for (int i = 0; i < entityDoorsOpenX.size(); i++)
	{
		file << "entityDoorOpenY" + std::to_string(i + 1) + "=\"" + std::to_string(entityDoorsOpenX[i]) + "\"," << std::endl;
	}
	for (int j = 0; j < entityDoorsOpenY.size(); j++)
	{
		file << "entityDoorOpenX" + std::to_string(j + 1) + "=\"" + std::to_string(entityDoorsOpenY[j]) + "\"," << std::endl;
	}
	file << "totalCloseDoors = \"" + std::to_string(totalCloseDoors) + "\"," << std::endl;
	file << "totalOpenDoors = \"" + std::to_string(totalOpenDoors) + "\"," << std::endl;
	file << "}" << std::endl;
	file.close();
}

void LuaReader::saveSwitches(std::vector<int> entitySwitchesX, std::vector<int> entitySwitchesY, std::vector<int> entitySwitchesUseX, std::vector<int> entitySwitchesUseY)
{
	ofstream file;
	file.open("Scripts//SaveSwitches.lua");
	file << "SaveSwitches= {" << std::endl;
	for (int j = 0; j < entitySwitchesY.size(); j++)
	{
		file << "entitySwitchX" + std::to_string(j + 1) + "=\"" + std::to_string(entitySwitchesY[j]) + "\"," << std::endl;
	}
	for (int i = 0; i < entitySwitchesX.size(); i++)
	{
		file << "entitySwitchY" + std::to_string(i + 1) + "=\"" + std::to_string(entitySwitchesX[i]) + "\"," << std::endl;
	}
	for (int j = 0; j < entitySwitchesUseY.size(); j++)
	{
		file << "entitySwitchUseX" + std::to_string(j + 1) + "=\"" + std::to_string(entitySwitchesUseY[j]) + "\"," << std::endl;
	}
	for (int i = 0; i < entitySwitchesUseX.size(); i++)
	{
		file << "entitySwitchUseY" + std::to_string(i + 1) + "=\"" + std::to_string(entitySwitchesUseX[i]) + "\"," << std::endl;
	}
	file << "}" << std::endl;
	file.close();
}

void LuaReader::saveEnemies(Vector3 position, Vector3 direction, Vector3 rotation, std::string state, int aiSightLength, int wayPointSize, std::vector<int> entityWaypointX, std::vector<int> entityWaypointY, int enemyListIndex, int currentLevel, int nextWaypointIndex, std::string difficulty)
{
	std::stringstream positionX, positionY;
	positionX.precision(2); positionY.precision(2);
	positionX << position.x; positionY << position.y;
	std::stringstream directionX, directionY, directionZ;
	directionX.precision(2); directionY.precision(2); directionZ.precision(2);
	directionX << direction.x; directionY << direction.y; directionZ << direction.z;
	std::stringstream rotationX, rotationY, rotationZ;
	rotationX.precision(2); rotationY.precision(2); rotationZ.precision(2);
	rotationX << rotation.x; rotationY << rotation.y; rotationZ << rotation.z;

	ofstream file;
	if (currentLevel == 3)
	{
		file.open("Scripts//Guards//GuardSave8.lua");
	}

	else
	{
		file.open("Scripts//Guards//GuardSave" + std::to_string(enemyListIndex + 1) + ".lua");
	}

	file << "Guard = {" << std::endl;
	file << "InformationComponent = {" << std::endl;
	file << "name = \"Guard\"," << std::endl;
	file << "position = {" + positionX.str() + "," + positionY.str() + "}," << std::endl;
	file << "direction = {" + directionX.str() + "," + directionY.str() + "," + directionZ.str() + "}," << std::endl;
	file << "rotation = {" + rotationX.str() + "," + rotationY.str() + "," + rotationZ.str() + "}," << std::endl;
	file << "type = \"NPC\"," << std::endl;
	file << "}," << std::endl;

	file << "GraphicsComponent = {" << std::endl;
	file << "meshType = \"Quad\"," << std::endl;
	file << "meshName = \"Guard Mesh\"," << std::endl;
	file << "meshImage = \"Images//guard.tga\"," << std::endl;
	file << "meshSize = 32," << std::endl;
	file << "meshColor = {1, 1, 1}," << std::endl;
	file << "meshAlpha = 1," << std::endl;
	file << "}," << std::endl;

	file << "AIComponent = {" << std::endl;
	file << "aiState = \"" + state + "\"," << std::endl;
	file << "aiType = \"Guard\"," << std::endl;
	file << "aiDifficulty = \"" + difficulty + "\"," << std::endl;
	file << "aiSightLength = " + std::to_string(aiSightLength) + "," << std::endl;
	file << "}," << std::endl;

	file << "WaypointComponent = {" << std::endl;
	file << "numberOfPoints = " + std::to_string(wayPointSize) + "," << std::endl;
	for (int i = 0; i < wayPointSize; i++)
	{
		if (nextWaypointIndex + i == wayPointSize)
		{
			nextWaypointIndex -= wayPointSize;
		}

		if (i + 2 == wayPointSize + 1)
		{
			file << "point1 = {" + std::to_string(entityWaypointY[nextWaypointIndex + i]) + "," + std::to_string(entityWaypointX[nextWaypointIndex + i]) + "}," << std::endl;
		}

		else
		{
			file << "point" + std::to_string(i + 2) + " = " + "{" + std::to_string(entityWaypointY[nextWaypointIndex + i]) + "," + std::to_string(entityWaypointX[nextWaypointIndex + i]) + "}," << std::endl;
		}

	}
	file << "}," << std::endl;

	file << "}" << std::endl;
	file.close();
}