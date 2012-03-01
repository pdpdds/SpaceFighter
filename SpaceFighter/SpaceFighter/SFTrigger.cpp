#include "StdAfx.h"
#include "SFTrigger.h"
#include "SFSystem.h"
#include "SFResMesh.h"
#include "OBB.h"
#include "SFAIPatternFactory.h"
#include "SFPlayerPlane.h"

SFTrigger::SFTrigger(void)
{
}

SFTrigger::~SFTrigger(void)
{
}

BOOL SFTrigger::Init( GameObjectDesc& Desc )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();

	SetPosition(Desc.vecStartPos);
	SetVelocity(Desc.vecVel);

	SetTimeLine(Desc.TimeLine);
	SFResMesh* pRes = (SFResMesh*)pSystem->GetResMgr()->FindResource(Desc.ResID);
	SetResource(pRes);

	SetInteract(TRUE);
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

BOOL SFTrigger::Render( float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	IDirect3DDevice9* pD3DDevice = pSystem->GetD3DDevice();

	D3DXMATRIX matWorld, matTranslation;
	pD3DDevice->GetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3* pPos = GetPosition();
	D3DXMATRIX* pRot = GetRotation();
	D3DXMatrixTranslation(&matTranslation, pPos->x, pPos->y, pPos->z);	

	D3DXMATRIX  Rotation = *pRot;

	matWorld =  Rotation * matTranslation;// * // * matScaling;
	pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
	pD3DDevice->SetRenderState( D3DRS_AMBIENT, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	GetResource()->Render(fElapsedTime, this);

	return TRUE;
}

BOOL SFTrigger::Update( float fElapsedTime )
{
	OBB* pOBB = GetOBB();

	D3DXQUATERNION rot;

	D3DXQuaternionRotationMatrix(&rot, GetRotation());

	pOBB->m_rot += rot * fElapsedTime;

	D3DXQuaternionNormalize(&pOBB->m_rot, &pOBB->m_rot);
	pOBB->m_pos = *GetPosition();

	return TRUE;
}

BOOL SFTrigger::ProcessInteraction( GameObject* pObject )
{
	if(pObject->GetObjectType() != OBJECT_PLAYER)
		return FALSE;
	SFPlayerPlane* pPlane = (SFPlayerPlane*)pObject;

	WeaponInfo Info;
	Info.WeaponDelay = 0;
	Info.MaxFireObjectCount = 0;
	Info.vecSize = D3DXVECTOR3(6.0f, 6.0f, 6.0f);
	Info.ResourceID = EXPLSION_EFFECT_1;
	Info.vecVelocity = D3DXVECTOR3(0,0,80);
	Info.Damage = 2;
	pPlane->GetWeaponSystem()->AddWeapon(WEAPON_FIREWORK, Info);

	//Info.vecVelocity = D3DXVECTOR3(0,0,160);
	//GetWeaponSystem()->AddWeapon(WEAPON_TWO, Info);
	
	
	delete this;

	return TRUE;
}