#include "StdAfx.h"
#include "SFScene1.h"
#include "SFBackGroundTerrain.h"
#include "SFBossBigBird.h"
#include "SFPlayerPlane.h"
#include "SFNormalPlane.h"
#include "SFSystem.h"
#include "Sound/BassSoundManager.h"
#include "SFBackGroundWater.h"
#include "lua.hpp"
#include "SFLuaBind.h"
#include "SFBackGroundParticle.h"

SFScene1::SFScene1(void)
{
}

SFScene1::~SFScene1(void)
{
}

BOOL SFScene1::Init()
{
	SFBackGround* pBackGround = new SFBackGroundTerrain();
	pBackGround->Init();

	AddBackGround(pBackGround);

	SFPlayerPlane* pPlayer = new SFPlayerPlane();
	GameObjectDesc Desc;
	Desc.ResID = HERO_PLANE;
	pPlayer->Init(Desc);
	pPlayer->SetObjectType(OBJECT_PLAYER);
	pPlayer->SetPosition(D3DXVECTOR3(20.0f,0.3f,0.0f));
	pPlayer->SetVelocity(D3DXVECTOR3(80.0f,80.0f,80.0f));
	
	SetHandlingObject(pPlayer);

	SetWorldSize(D3DXVECTOR4(-100, 100, -75, 80));

	return LoadGameObject();
}

BOOL SFScene1::UpdateSpecificScene(float fElapsedTime )
{
	return TRUE;
}

BOOL SFScene1::LoadGameObject()
{
	///////////////////////////////////////////////////////////////////////////////////////////
	//Lua Data
	///////////////////////////////////////////////////////////////////////////////////////////
	lua_State* L = lua_open();

	// Lua �⺻ �Լ����� �ε��Ѵ�.- print() ���
	luaopen_base(L);

	RegisterAllLuaBinds(L);

	// sample1.lua ������ �ε�/�����Ѵ�.
	lua_tinker::dofile(L, "scene1.lua");

	// sample1.lua �� �Լ��� ȣ���Ѵ�.
	//int result = lua_tinker::call<int>(L, "lua_func", 3, 4);

	// lua_func(3,4) �� ����� ���
//	printf("lua_func(3,4) = %d\n", result);

	// ���α׷� ����
	lua_close(L);

	return TRUE;
}



