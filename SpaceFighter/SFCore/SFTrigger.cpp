#include "StdAfx.h"
#include "SFTrigger.h"
#include "SFSystem.h"
#include "SFResMesh.h"
#include "OBB.h"
#include "SFAIPatternFactory.h"
#include "SFPlayerPlane.h"
#include "SDKSound.h"

extern CSound* g_pSoundGetItem;

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

	SetInteract(FALSE);


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

BOOL SFTrigger::Render( float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	IDirect3DDevice9* pD3DDevice = pSystem->GetD3DDevice();
	
	pD3DDevice->BeginScene();

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

	/*TCHAR strPass[256];
	_stprintf_s(strPass, L"%f,%f,%f",pPos->x, pPos->y, pPos->z);

	RECT rc;
	SetRect( &rc, 0, 0, 300, 0 );
	pSystem->GetDebugFont()->Print(strPass, rc,  D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));*/

	D3DXMATRIX matView, matProj;
	pD3DDevice->GetTransform( D3DTS_VIEW, &matView );
	pD3DDevice->GetTransform( D3DTS_PROJECTION, &matProj );

	matProj = matView * matProj;
	GetOBB()->Render(matProj);

	pD3DDevice->EndScene();
	return TRUE;
}

BOOL SFTrigger::Update( float fElapsedTime )
{
	SFPlane::Update(fElapsedTime);

	return TRUE;
}

BOOL SFTrigger::ProcessInteraction( GameObject* pObject )
{
	if(pObject->GetObjectType() != OBJECT_PLAYER)
		return FALSE;

	SFPlayerPlane* pPlane = (SFPlayerPlane*)pObject;

	int iWeapon = rand() % 4;

	WeaponInfo Info;

	Info.WeaponDelay = 0;
	Info.MaxFireObjectCount = 0;
	Info.vecSize = D3DXVECTOR3(6.0f, 6.0f, 6.0f);
	Info.ResourceID = EXPLSION_EFFECT_1;
	Info.vecVelocity = D3DXVECTOR3(0,0,80);
	Info.Damage = 2;

	if(iWeapon == 0)
		pPlane->GetWeaponSystem()->AddWeapon(WEAPON_THREE, Info);
	if(iWeapon == 0)
		pPlane->GetWeaponSystem()->AddWeapon(WEAPON_FIREWORK, Info);
	else
		pPlane->GetWeaponSystem()->AddWeapon(WEAPON_TWO, Info);


	g_pSoundGetItem->Play( 0, 0L);

	//Info.vecVelocity = D3DXVECTOR3(0,0,160);
	//GetWeaponSystem()->AddWeapon(WEAPON_TWO, Info);
	
	return TRUE;
}