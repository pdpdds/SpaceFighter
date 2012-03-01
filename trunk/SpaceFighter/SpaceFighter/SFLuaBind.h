#pragma once
extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include "lua_tinker.h"

void RegisterAllLuaBinds(lua_State* L);

