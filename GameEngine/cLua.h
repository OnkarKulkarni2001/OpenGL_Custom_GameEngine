#pragma once
#include <Lua/lua.hpp>

class cLua
{
public:
	void InitLua(lua_State* state);
	void ExecuteLuaScript(lua_State* state, const char* script);
};