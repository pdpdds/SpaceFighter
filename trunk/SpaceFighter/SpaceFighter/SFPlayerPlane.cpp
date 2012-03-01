#include "StdAfx.h"
#include "SFPlayerPlane.h"
#include "DirectInput.h"
#include "SFSystem.h"
#include "SFWeapon.h"
#include "SFScene.h"
#include "OBB.h"
#include "SFResMesh.h"

SFPlayerPlane::SFPlayerPlane(void)
{
}

SFPlayerPlane::~SFPlayerPlane(void)
{
}

BOOL SFPlayerPlane::ProcessInput( float fElapsedTime )
{
	DirectInput* pInput = SFSystem::GetInstance()->GetInputSystem();
	D3DXVECTOR3* pPos = GetPosition();
	D3DXVECTOR3* pVel = GetVelocity();

	if( pInput->keyDown(DIK_W) )
		pPos->z += fElapsedTime * pVel->z;
	if( pInput->keyDown(DIK_S) )
		pPos->z -= fElapsedTime * pVel->z;
	if( pInput->keyDown(DIK_A) )
		pPos->x -= fElapsedTime * pVel->x;
	if( pInput->keyDown(DIK_D) )
		pPos->x += fElapsedTime * pVel->x;
	if( pInput->keyDown(DIK_SPACE) )
	{
		GetWeaponSystem()->CreateFireObject(fElapsedTime);
	}
	if( pInput->keyDown(DIK_Z) )
	{
		GetWeaponSystem()->ChangeWeapon();
	}

	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	SFScene* pScene = pSystem->GetCurrentScene();
	D3DXVECTOR4 SceneArea = pScene->GetWorldSize();

	if(pPos->x < SceneArea.x + 10)
		pPos->x = SceneArea.x + 10;
	if(pPos->x > SceneArea.y - 10)
		pPos->x = SceneArea.y - 10;
	if(pPos->z < SceneArea.z + 10)
		pPos->z = SceneArea.z + 10;
	if(pPos->z > SceneArea.w - 10)
		pPos->z = SceneArea.w - 10;

	return TRUE;
}

BOOL SFPlayerPlane::Render( float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	IDirect3DDevice9* pD3DDevice = pSystem->GetD3DDevice();

	D3DXMATRIX matWorld, matScaling, matTranslation;
	pD3DDevice->GetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3* pPos = GetPosition();
	D3DXMATRIX* pRot = GetRotation();
	D3DXMatrixTranslation(&matTranslation, pPos->x, pPos->y, pPos->z);	
	D3DXMatrixScaling(&matScaling, 1, 1, 1);

	D3DXMATRIX matRot;
	D3DXMatrixRotationY(&matRot, PI);

	D3DXMATRIX  Rotation = *pRot;

	matWorld =  matRot * matTranslation;// * // * matScaling;
	pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
	pD3DDevice->SetRenderState( D3DRS_AMBIENT, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	GetResource()->Render(fElapsedTime);

	TCHAR strPass[256];
	_stprintf_s(strPass, L"%f,%f,%f",pPos->x, pPos->y, pPos->z);

	RECT rc;
	SetRect( &rc, 0, 0, 300, 0 );
	pSystem->GetDebugFont()->Print(strPass, rc,  D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));

	return TRUE;
}

BOOL SFPlayerPlane::Update( float fElapsedTime )
{
	OBB* pOBB = GetOBB();

	D3DXQUATERNION rot;

	D3DXQuaternionRotationMatrix(&rot, GetRotation());

	pOBB->m_rot += rot * fElapsedTime;

	D3DXQuaternionNormalize(&pOBB->m_rot, &pOBB->m_rot);
	pOBB->m_pos = *GetPosition();

	return FALSE;
}

BOOL SFPlayerPlane::Init(GameObjectDesc& Desc)
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();

	WeaponInfo Info;
	Info.WeaponDelay = 0;
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
	return TRUE;
}