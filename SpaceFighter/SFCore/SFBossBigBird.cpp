#include "StdAfx.h"
#include "SFBossBigBird.h"
#include "SFSystem.h"
#include "SFResMesh.h"
#include "OBB.h"
#include "SFAIPatternFactory.h"

SFBossBigBird::SFBossBigBird(void)
{
}

SFBossBigBird::~SFBossBigBird(void)
{
}

BOOL SFBossBigBird::Render( float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	IDirect3DDevice9* pD3DDevice = pSystem->GetD3DDevice();
	
	D3DXMATRIX matWorld, matScaling, matTranslation;
	pD3DDevice->GetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3* pPos = GetPosition();
	D3DXMATRIX* pRot = GetRotation();
	D3DXMatrixTranslation(&matTranslation, pPos->x, pPos->y, pPos->z);	
	D3DXMatrixScaling(&matScaling, 1, 1, 1);

	D3DXMATRIX  Rotation = *pRot;

	matWorld =  Rotation * matTranslation;// * // * matScaling;
	pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
	pD3DDevice->SetRenderState( D3DRS_AMBIENT, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	GetResource()->Render(fElapsedTime);

	return TRUE;
}

BOOL SFBossBigBird::Update(float fElapsedTime)
{
	SFPlane::Update(fElapsedTime);
	return TRUE;
}

BOOL SFBossBigBird::Init(GameObjectDesc& Desc)
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();

	SetPosition(Desc.vecStartPos);
	SetVelocity(Desc.vecVel);

	SetTimeLine(Desc.TimeLine);
	SFResMesh* pRes = (SFResMesh*)pSystem->GetResMgr()->FindResource(Desc.ResID);
	SetResource(pRes);

	WeaponInfo Info;
	Info.WeaponDelay = 0;
	Info.MaxFireObjectCount = 0;
	Info.vecSize = D3DXVECTOR3(6.0f, 6.0f, 6.0f);
	Info.vecVelocity = D3DXVECTOR3(0,0,-160);
	Info.ResourceID = EXPLSION_EFFECT_1;
	Info.Damage = 2;

	GetWeaponSystem()->AddWeapon(WEAPON_DEFAULT, Info);


	///////////////////////////////////////////////////////////////////////////////////////////
	//AI Pattern Add
	///////////////////////////////////////////////////////////////////////////////////////////

	SFAIPattern* pPattern = SFAIPatternFactory::CreateAIPattern(Desc.szAIPatternName);

	if(pPattern != NULL)
	{
		//SFASSERT(1);
		AddPattern(pPattern);
		Activate();
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////
	//OBB Creation
	///////////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 vecSize;
	vecSize = pRes->GetMax() - pRes->GetMin();
	vecSize /= 2.0f;
	OBB* pOBB = new OBB(vecSize);
	pOBB->m_pos = pRes->GetCenter();
	SetOBB(pOBB);

	return TRUE;
}

BOOL SFBossBigBird::ProcessInteraction( GameObject* pObject )
{
	return TRUE;
}