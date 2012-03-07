#include "StdAfx.h"
#include "SFPlayerLive.h"
#include "SFSystem.h"
#include "SFWeapon.h"
#include "SFScene.h"
#include "SFPlayerPlane.h"
#include "DirectInput.h"
#include "SFPlayerFSM.h"

SFPlayerLive::SFPlayerLive(SFPlayerPlane* pOwner)
: SFPlayerState(pOwner)
, m_NextFireTime(0)
{
}

SFPlayerLive::~SFPlayerLive(void)
{
}

BOOL SFPlayerLive::OnRender(float fElapsedTime)
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	IDirect3DDevice9* pD3DDevice = pSystem->GetD3DDevice();

	D3DXMATRIX matWorld, matScaling, matTranslation;
	pD3DDevice->GetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3* pPos = GetOwner()->GetPosition();
	D3DXMATRIX* pRot = GetOwner()->GetRotation();
	D3DXMatrixTranslation(&matTranslation, pPos->x, pPos->y, pPos->z);	
	D3DXMatrixScaling(&matScaling, 1, 1, 1);

	D3DXMATRIX matRot;
	D3DXMatrixRotationY(&matRot, PI);

	D3DXMATRIX  Rotation = *pRot;

	matWorld =  matRot * matTranslation;// * // * matScaling;
	pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
	pD3DDevice->SetRenderState( D3DRS_AMBIENT, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	GetOwner()->GetResource()->Render(fElapsedTime);

	TCHAR strPass[256];
	_stprintf_s(strPass, L"%f,%f,%f",pPos->x, pPos->y, pPos->z);

	RECT rc;
	SetRect( &rc, 0, 0, 300, 0 );
	pSystem->GetDebugFont()->Print(strPass, rc,  D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));

	return TRUE;
}

BOOL SFPlayerLive::OnUpdate(float fElapsedTime)
{
	OBB* pOBB = GetOwner()->GetOBB();

	D3DXQUATERNION rot;

	D3DXQuaternionRotationMatrix(&rot, GetOwner()->GetRotation());

	pOBB->m_rot += rot * fElapsedTime;

	D3DXQuaternionNormalize(&pOBB->m_rot, &pOBB->m_rot);
	pOBB->m_pos = *(GetOwner()->GetPosition());

	return TRUE;
}

BOOL SFPlayerLive::ProcessInput(float fElapsedTime)
{
	DirectInput* pInput = SFSystem::GetInstance()->GetInputSystem();
	D3DXVECTOR3* pPos = GetOwner()->GetPosition();
	D3DXVECTOR3* pVel = GetOwner()->GetVelocity();

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
		if(m_NextFireTime <= GetTickCount())
		{
			GetOwner()->GetWeaponSystem()->CreateFireObject(fElapsedTime);
			m_NextFireTime = GetTickCount() + 100;
		}
	}
	if( pInput->keyDown(DIK_Z) )
	{
  		GetOwner()->GetWeaponSystem()->ChangeWeapon();
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

BOOL SFPlayerLive::OnCheckInterAction(SFScene* pScene)
{
	SFScene::listPlane* pPlaneList = pScene->GetPlaneList();
	SFScene::listWeapon* pWeaponList = pScene->GetEnemyWeaponList();

	SFScene::listPlane::iterator iter = pPlaneList->begin();
	for(;iter!= pPlaneList->end(); iter++)
	{
		SFPlane* pPlane = (*iter);
		if(TRUE == GetOwner()->CheckIntersection(pPlane))
		{
		
			if(pPlane->GetObjectType() != OBJECT_TRIGGER)
				GetOwner()->GetFSM()->ChangeState(SF_PLAYERSTATE_DIE);

			if(TRUE == pPlane->ProcessInteraction(GetOwner()))
			{
				delete pPlane;
				
				pPlaneList->erase(iter);
			}

			return TRUE;
		}
	}

	SFScene::listWeapon::iterator iterFireObject = pWeaponList->begin();
	for(;iterFireObject!= pWeaponList->end(); iterFireObject++)
	{
		SFWeapon* pWeapon = (*iterFireObject);
		if(TRUE == GetOwner()->CheckIntersection(pWeapon))
		{
			delete pWeapon;
			pWeaponList->erase(iterFireObject);

			GetOwner()->GetFSM()->ChangeState(SF_PLAYERSTATE_DIE);
		
			return TRUE;
		}
	}
 
	return TRUE;
}
