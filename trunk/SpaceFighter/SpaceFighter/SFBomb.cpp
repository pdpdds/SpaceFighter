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
//AI ������ ���� �ʰ� ������Ʈ ������ �������� �����ϰ� ���� �� Update �Լ��� ������ �� �ֵ��� ����
//�θ��� Update�Լ��� ȣ����� �ʵ��� ������ ��
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