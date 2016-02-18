#ifndef LUA_READER_H
#define LUA_READER_H

#include <iostream>
#include <string>
#include <vector>
#include "Entity.h"
#include <fstream>

using std::fstream;
using std::ios;

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#pragma warning(disable:4800)

class LuaReader
{
public:
	LuaReader(const std::string & filepath);
	virtual ~LuaReader();

	inline void Clean()
	{
		int top = lua_gettop(L);
		lua_pop(L, top);
	}

	// Get Functions 
	template<typename T> T get(const std::string & variableName)
	{
		// No Script
		if (!L)
		{
			printError(variableName, "No script loaded");
			return lua_getdefault<T>();
		}

		T result;

		if (lua_gettostack(variableName))
		{
			result = lua_get<T>(variableName);
		}
		else
		{
			result = lua_getdefault<T>();
		}

		this->Clean();
		return result;
	}
	template<typename T> std::vector<T> getContents(const std::string & contentName)
	{
		std::vector<T> resultingVector;

		lua_gettostack(contentName.c_str());

		if (lua_isnil(L, -1))
		{
			// No Array
			printError(contentName, "There isnt anything in here!");
			return std::vector<T>();
		}
		lua_pushnil(L);

		while (lua_next(L, -2))
		{
			if (lua_isnumber(L, -1))
			{
				resultingVector.push_back((T)lua_tonumber(L, -1));
			}
			else if (lua_isstring(L, -1))
			{
				resultingVector.push_back((T)lua_tostring(L, -1));
			}
			lua_pop(L, 1);
		}
		this->Clean();
		return resultingVector;
	}

	template<typename T> T lua_get(const std::string & variableName) { return 0; }
	template<typename T> T lua_getdefault() { return 0; }
	template<typename T> std::vector<T> lua_getcontents(const std::string & contentName) { return 0 };
	template<typename T> T lua_to(T);
	std::vector<std::string> getTables(const std::string & tableName);
	bool lua_gettostack(const std::string& variableName);
	void saveFile();
	
	// Entity Creation (Test!!!)
	Entity * createEntity(const std::string & entityType, Camera * camera);

	// Print Error & Reason
	void printError(const std::string & variableName, const std::string reason);
private:
	lua_State * L;
	std::string filename;
	int currLevel;
};

template<> inline bool LuaReader::lua_get<bool>(const std::string & variableName)
{
	if (!lua_isnumber(L, -1))
	{
		printError(variableName, "Not a number");
	}
	return (bool)(lua_toboolean(L, -1));
}

template<> inline int LuaReader::lua_get<int>(const std::string & variableName)
{
	if (!lua_isnumber(L, -1))
	{
		printError(variableName, "Not a number");
	}
	return (int)(lua_tonumber(L, -1));
}

template<> inline float LuaReader::lua_get<float>(const std::string & variableName)
{
	if (!lua_isnumber(L, -1))
	{
		printError(variableName, "Not a number");
	}
	return (float)(lua_tonumber(L, -1));
}

template<> inline std::string LuaReader::lua_get<std::string>(const std::string & variableName)
{
	std::string temp = "temp";
	if (lua_isstring(L, -1))
	{
		temp = (std::string)(lua_tostring(L, -1));
	}
	else
	{
		printError(variableName, "Not a string");
	}
	return temp;
}

template<> inline Vector3 LuaReader::lua_get<Vector3>(const std::string & variableName)
{
	lua_gettostack(variableName.c_str());

	if (lua_isnil(L, -1))
	{
		printError(variableName, "Not a array of numbers");
	}
	lua_pushnil(L);

	std::vector<float> temp;

	while (lua_next(L, -2))
	{
		if (lua_isnumber(L, -1))
		{
			temp.push_back((float)lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
	}
	return Vector3(temp[0], temp[1], temp[2]);
}

template<> inline Color LuaReader::lua_get<Color>(const std::string & variableName)
{
	lua_gettostack(variableName.c_str());

	if (lua_isnil(L, -1))
	{
		printError(variableName, "Not a array of numbers");
	}
	lua_pushnil(L);

	std::vector<float> temp;

	while (lua_next(L, -2))
	{
		if (lua_isnumber(L, -1))
		{
			temp.push_back((float)lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
	}
	return Color(temp[0], temp[1], temp[2]);
}

template<> inline std::string LuaReader::lua_getdefault<std::string>()
{
	return "undefined";
}



#endif