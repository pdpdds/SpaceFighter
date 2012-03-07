#include "StdAfx.h"
#include "SFPlayerPlane.h"
#include "SFSystem.h"
#include "SFWeapon.h"
#include "SFScene.h"
#include "OBB.h"
#include "SFResMesh.h"
#include "SFPlayerFSM.h"

SFPlayerPlane::SFPlayerPlane(void)
{
	
}

SFPlayerPlane::~SFPlayerPlane(void)
{
}

BOOL SFPlayerPlane::ProcessInput( float fElapsedTime )
{
	return m_pFSM->ProcessInput(fElapsedTime);
}

BOOL SFPlayerPlane::Render( float fElapsedTime )
{
	return m_pFSM->OnRender(fElapsedTime);
}

BOOL SFPlayerPlane::Update( float fElapsedTime )
{
	return m_pFSM->OnUpdate(fElapsedTime);
}

BOOL SFPlayerPlane::Init(GameObjectDesc& Desc)
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();

	WeaponInfo Info;
	Info.WeaponDelay = 100;
	Info.MaxFireObjectCount = 0;
	Info.vecSize = D3DXVECTOR3(6.0f, 6.0f, 6.0f);
	Info.vecVelocity = D3DXVECTOR3(0,0,160);
	Info.ResourceID = EXPLSION_EFFECT_1;
	Info.Damage = 2;

	GetWeaponSystem()->AddWeapon(WEAPON_DEFAULT, Info);

	SFResMesh* pRes = (SFResMesh*)pSystem->GetResMgr()->FindResource(Desc.ResID);
	SetResource(pRes);

	///////////////////////////////////////////////////////////////////////////////////////////
	//OBB Creation
	///////////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 vecSize;
	vecSize = pRes->GetMax() - pRes->GetMin();
	vecSize /= 2.0f;
	OBB* pOBB = new OBB(vecSize);
	pOBB->m_pos = pRes->GetCenter();
	SetOBB(pOBB);

///////////////////////////////////////////////////////////////////////////////////////////
//FSM Creation
///////////////////////////////////////////////////////////////////////////////////////////
	m_pFSM = new SFPlayerFSM(this);
	m_pFSM->AddState(SF_PLAYERSTATE_INIT);
	m_pFSM->AddState(SF_PLAYERSTATE_LIVE);
	m_pFSM->AddState(SF_PLAYERSTATE_DIE);

	m_pFSM->ChangeState(SF_PLAYERSTATE_INIT);
	m_pFSM->ChangeState(SF_PLAYERSTATE_LIVE);

	return TRUE;
}