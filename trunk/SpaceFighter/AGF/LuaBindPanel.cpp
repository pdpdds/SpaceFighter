#include "StdAfx.h"
#include "lua.hpp"
#include "LuaBindPanel.h"
#include "DirectXSystem.h"
#include "PannelManager.h"
#include "Pannel.h"

int Lua_AddPannel(lua_State *L)
{
	int PannelID	= luaL_checkinteger(L, -1);

	CPannel* pPannel = new CPannel(PannelID);

	CDirectXSystem::GetInstance()->GetPannelManager()->CreatePannel(pPannel);

	return 0;
}

int Lua_AddPannelComponent(lua_State *L)
{
	ComponentInfo Info;
	Info.PannelID = luaL_checkinteger(L, -10);
	Info.iComponentType = luaL_checkinteger(L, -9);
	Info.iIdentifier = luaL_checkinteger(L, -8);
	Info.iPosX = luaL_checkinteger(L, -7);
	Info.iPosY = luaL_checkinteger(L, -6);
	Info.iWidth = luaL_checkinteger(L, -5);
	Info.iHeight = luaL_checkinteger(L, -4);

	const char* szCaption = luaL_checkstring(L, -3);
    WCHAR strName[LEN_GUI_CAPTION];

    MultiByteToWideChar( CP_ACP, 0, szCaption, -1, strName, LEN_GUI_CAPTION );
    strName[LEN_GUI_CAPTION-1] = 0;

	_tcscpy(Info.szGUICaption, strName);

	Info.iButtonGroup = luaL_checkinteger(L, -2);
	Info.bDefault = (luaL_checkinteger(L, -1) != 0);
	
	CDirectXSystem::GetInstance()->GetPannelManager()->CreatePannelComponent(&Info);

	return 0;
}

void RegisterAllLuaBinds(lua_State* L)
{
	lua_register(L, "LuaAddPannel", Lua_AddPannel);
	lua_register(L, "LuaAddPannelComponent", Lua_AddPannelComponent);
};

