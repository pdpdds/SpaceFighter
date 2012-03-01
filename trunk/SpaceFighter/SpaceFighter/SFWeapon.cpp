#include "StdAfx.h"
#include "SFWeapon.h"
#include "SFSystem.h"


SFWeapon::SFWeapon( GameObject* pOwner, WeaponInfo& Info )
: m_pOwner(pOwner)
, m_eMode(Morphing_0)
{
	m_WeaponInfo = Info;

	m_WeaponInfo.FireTime = timeGetTime();

	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	SFRes* pRes = pSystem->GetResMgr()->FindResource(Info.ResourceID);

	if(pRes == NULL)
	{
		SFASSERT(1);
	}

	SetResource(pRes);
	SetVelocity(Info.vecVelocity);
	SetPosition(Info.vecStartPos);

///////////////////////////////////////////////////////////////////////////////////////////
//OBB Creation
///////////////////////////////////////////////////////////////////////////////////////////
	OBB* pOBB = new OBB(m_WeaponInfo.vecSize);
	pOBB->m_pos = D3DXVECTOR3(0,0,0);
	SetOBB(pOBB);

	Timer.Start();
}

SFWeapon::~SFWeapon(void)
{
}

BOOL SFWeapon::Render( float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	IDirect3DDevice9* pD3DDevice = pSystem->GetD3DDevice();

	D3DXMATRIX matWorld, matTranslation;

	D3DXVECTOR3* pPos = GetPosition();
	D3DXMATRIX* pRot = GetRotation();
	D3DXMatrixTranslation(&matTranslation, pPos->x, pPos->y, pPos->z);	

	D3DXMATRIX  Rotation = *pRot;

	matWorld =  Rotation * matTranslation;
	pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	GetResource()->Render(fElapsedTime, this, Timer);

	return TRUE;
}

BOOL SFWeapon::Update( float fElapsedTime )
{
	OBB* pOBB = GetOBB();

	D3DXQUATERNION rot;

	D3DXQuaternionRotationMatrix(&rot, GetRotation());

	pOBB->m_rot += rot * fElapsedTime;

	D3DXQuaternionNormalize(&pOBB->m_rot, &pOBB->m_rot);
	pOBB->m_pos = *GetPosition();

	UpdateWeapon(fElapsedTime);

	Timer.Update(fElapsedTime);

	return TRUE;
}

BOOL SFWeapon::OnMessageEvent( GameObject* pSender, SFMessage& Msg )
{
	if(Msg.MessageID == MSG_PLANE_DIE)
	{
		if(pSender == GetOwner())
			SetOwner(NULL);
	}

	return TRUE;
}