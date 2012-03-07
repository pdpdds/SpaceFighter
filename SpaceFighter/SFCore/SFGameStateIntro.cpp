#include "StdAfx.h"
#include "SFGameStateIntro.h"
#include "SFSystem.h"
#include "ScreenMgr.h"
#include "SFGameStateMachine.h"

SFGameStateIntro::SFGameStateIntro(void)
{
}

SFGameStateIntro::~SFGameStateIntro(void)
{
}

BOOL SFGameStateIntro::OnRender( float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	pSystem->GetScreenMgr()->DoIntro(fElapsedTime);

	return TRUE;
}

BOOL SFGameStateIntro::OnUpdate( float fTime, float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	SFGameStateMachine* pMachine = pSystem->GetGameStateMachine();

	if(pSystem->GetScreenMgr()->GetIntroEnd())
	{
		D3DXMATRIX mat;  		
		D3DXMatrixIdentity(&mat);  		

		pSystem->GetD3DDevice()->SetTransform( D3DTS_PROJECTION, pSystem->GetProjMatrix() );
		pSystem->GetD3DDevice()->SetTransform( D3DTS_WORLD, &mat ); 

		pMachine->ChangeState(SF_GAMESTATE_PLAY);
	}

	return TRUE;
}