#include "StdAfx.h"
#include "SFGameStatePlay.h"
#include "SFSystem.h"
#include "SFScene.h"
#include "SFScene1.h"

SFGameStatePlay::SFGameStatePlay(void)
{
}

SFGameStatePlay::~SFGameStatePlay(void)
{
}

BOOL SFGameStatePlay::OnRender( float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	SFScene* pScene = pSystem->GetCurrentScene();
	pScene->Render(fElapsedTime);

	return TRUE;
}

BOOL SFGameStatePlay::OnUpdate(float fTime, float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	SFScene* pScene = pSystem->GetCurrentScene();
	pScene->Update(fTime, fElapsedTime);

	return TRUE;
}

BOOL SFGameStatePlay::OnEnter()
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	SFScene* pScene = pSystem->GetCurrentScene();

	if(NULL == pScene)
	{
		pScene = new SFScene1();
		pSystem->SetCurrentScene(pScene);
		pScene->Init();
	}

	return TRUE;
}