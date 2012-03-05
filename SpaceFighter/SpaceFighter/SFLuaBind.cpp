#include "StdAfx.h"
#include "lua.hpp"
#include "SFLuaBind.h"
#include "SFSystem.h"
#include "SFBossBigBird.h"
#include "SFScene.h"
#include "SFObjectFactory.h"

void AddPlanePool(lua_tinker::table ExternalDesc)
{
	static int id = 0;
	id++;
	GameObjectDesc Desc;

	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	SFScene* pScene = pSystem->GetCurrentScene();
	
	//strcpy(b.str, GameObjectDesc.get<const char*>("str"));
	Desc.ObjectType = ExternalDesc.get<int>("ObjectType");
	Desc.ResID = ExternalDesc.get<int>("ResID");
	Desc.vecStartPos.x = ExternalDesc.get<float>("PosX");
	Desc.vecStartPos.y = ExternalDesc.get<float>("PosY");
	Desc.vecStartPos.z = ExternalDesc.get<float>("PosZ"); 
	Desc.vecVel.x = ExternalDesc.get<float>("VelX");
	Desc.vecVel.y = ExternalDesc.get<float>("VelY");
	Desc.vecVel.z = ExternalDesc.get<float>("VelZ"); 
    Desc.TimeLine = ExternalDesc.get<DWORD>("TimeLine");
	memcpy(Desc.szAIPatternName, ExternalDesc.get<const char*>("AIPattern"), MAX_PATH);

	SFPlane* pPlane = SFObjectFactory::CreateGameObject(Desc.ObjectType);

	pPlane->Init(Desc);
	pPlane->SetObjectType(Desc.ObjectType);
	pPlane->SetTarget(pScene->GetHandlingObject());

	pScene->AddPlanePool(id, pPlane);
}


void RegisterAllLuaBinds(lua_State* L)
{
	// LuaTinker 를 이용해서 함수를 등록한다.
	lua_tinker::def(L, "AddPlanePool", AddPlanePool);
};

