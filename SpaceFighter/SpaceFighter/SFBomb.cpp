#include "StdAfx.h"
#include "SFBomb.h"
#include "SFSystem.h"
#include "SFResMesh.h"
#include "OBB.h"
#include "SFAIPatternFactory.h"

SFBomb::SFBomb(void)
: m_fTime(0.0f)
, m_bfEffect(FALSE)
, m_ScaleFactor(1.0f)
{
	D3DXMatrixIdentity( &m_matScaling );
}

SFBomb::~SFBomb(void)
{
}

BOOL SFBomb::Init( GameObjectDesc& Desc )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();

	SetPosition(Desc.vecStartPos);
	SetVelocity(Desc.vecVel);

	SetTimeLine(Desc.TimeLine);
	SFRes* pRes = (SFResMesh*)pSystem->GetResMgr()->FindResource(Desc.ResID);
	SetResource(pRes);

	SetInteract(FALSE);
	///////////////////////////////////////////////////////////////////////////////////////////
	//OBB Creation
	///////////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 vecSize;
	vecSize = D3DXVECTOR3(4.0f, 4.0f, 4.0f);
	vecSize /= 2.0f;
	OBB* pOBB = new OBB(vecSize);
	pOBB->m_pos = D3DXVECTOR3(0,0,0);
	SetOBB(pOBB);

	m_ScaleFactor = GetScale();

	return TRUE;
}

BOOL SFBomb::Render( float fElapsedTime )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	IDirect3DDevice9* pD3DDevice = pSystem->GetD3DDevice();

	D3DXMATRIX matWorld, matTranslation;
	pD3DDevice->GetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3* pPos = GetPosition();
	D3DXMATRIX* pRot = GetRotation();
	D3DXMatrixTranslation(&matTranslation, pPos->x, pPos->y, pPos->z);	

	D3DXMATRIX  Rotation = *pRot;

	matWorld =  Rotation * matTranslation * m_matScaling;// * // * matScaling;
	pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
	pD3DDevice->SetRenderState( D3DRS_AMBIENT, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	GetResource()->Render(fElapsedTime, this);

	D3DXMATRIX matView, matProj;
	pD3DDevice->GetTransform( D3DTS_VIEW, &matView );
	pD3DDevice->GetTransform( D3DTS_PROJECTION, &matProj );

	matProj = matView * matProj;
	GetOBB()->Render(matProj);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//AI 패턴을 쓰지 않고 오브젝트 고유의 패턴으로 정의하고 싶을 때 Update 함수를 수정할 수 있도록 하자
//부모의 Update함수가 호출되지 않도록 주의할 것
////////////////////////////////////////////////////////////////////////////////////
BOOL SFBomb::Update( float fElapsedTime )
{
	m_fTime += fElapsedTime;

	if(m_fTime > 3.0f)
	{
		SetInteract(TRUE);

		if(m_fTime < 4.0f)
		{
			m_ScaleFactor += fElapsedTime * 80.0f;
			
			SetScale(m_ScaleFactor);
		}

		if(m_fTime > 8.0f)
		{
			SetDisable(TRUE);
		}
	}

	return TRUE;
}