#include "StdAfx.h"
#include "SFResAni.h"
#include "SFSystem.h"
#include "Sprite.h"
#include "GameObject.h"

SFResAni::SFResAni( CSprite* pSprite )
: m_pSprite(pSprite)
{

}

SFResAni::~SFResAni(void)
{
}

BOOL SFResAni::Render( float fElapsedTime )
{
	static CTimer Timer;
	Timer.Start();
	Timer.Update(fElapsedTime);

	IDirect3DDevice9* pD3DDevice = SFSystem::GetInstance()->GetD3DDevice();
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE);	
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);      
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);      
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);   

	D3DXMATRIX TSWorld, TSView;
	D3DXMatrixIdentity( &TSWorld );
	
	pD3DDevice->GetTransform(D3DTS_VIEW, &TSView);

	static float m_fElapsedTime  = 0;
	m_fElapsedTime += fElapsedTime;

	m_pSprite->m_pAnim->BillboardRotation(TSView, 0, 7, -10, 20 );	 
	m_pSprite->m_pAnim->RenderBillboard(Timer, TSWorld);	  
	
	//m_pSprite->Render(TSView);

	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);      	
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	return TRUE;
}

BOOL SFResAni::Render( float fElapsedTime, GameObject* pObject)
{
	static CTimer Timer;
	Timer.Start();
	Timer.Update(fElapsedTime);

	IDirect3DDevice9* pD3DDevice = SFSystem::GetInstance()->GetD3DDevice();
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE);	
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);      
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);      
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);   

	D3DXMATRIX TSWorld, TSView;
	D3DXMatrixIdentity( &TSWorld );

	pD3DDevice->GetTransform(D3DTS_VIEW, &TSView);

	static float m_fElapsedTime  = 0;
	m_fElapsedTime += fElapsedTime;

	D3DXVECTOR3* pPos = pObject->GetPosition();

	m_pSprite->m_pAnim->BillboardRotation(TSView, pPos->x, 7, pPos->z, pObject->GetScale());	 
	m_pSprite->m_pAnim->RenderBillboard(Timer, TSWorld);	  

	//m_pSprite->Render(TSView);

	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);      	
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	return TRUE;
}

BOOL SFResAni::Render( float fElapsedTime, GameObject* pObject, CTimer& Timer )
{
	IDirect3DDevice9* pD3DDevice = SFSystem::GetInstance()->GetD3DDevice();
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE);	
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);      
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);      
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);   

	D3DXMATRIX TSWorld, TSView;
	D3DXMatrixIdentity( &TSWorld );

	pD3DDevice->GetTransform(D3DTS_VIEW, &TSView);

	static float m_fElapsedTime  = 0;
	m_fElapsedTime += fElapsedTime;

	D3DXVECTOR3* pPos = pObject->GetPosition();

	m_pSprite->m_pAnim->BillboardRotation(TSView, pPos->x, 7, pPos->z, pObject->GetScale());	 
	m_pSprite->m_pAnim->RenderBillboard(Timer, TSWorld);	  

	//m_pSprite->Render(TSView);

	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);      	
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	return TRUE;
}

BOOL SFResAni::Init()
{
	return FALSE;
}