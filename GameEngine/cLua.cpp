#include "cLua.h"
#include <iostream>

void cLua::InitLua(lua_State* state)
{
	luaL_openlibs(state);
	
	// Need to bind C++ functions in here
}

void cLua::ExecuteLuaScript(lua_State* state, const char* script)
{
	if (luaL_dofile(state, script) != LUA_OK) {
		const char* error = lua_tostring(state, -1);
		std::cerr << "Error loading Lua script: " << error << std::endl;
	}
}
