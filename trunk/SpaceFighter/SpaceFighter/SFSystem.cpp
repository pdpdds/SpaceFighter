#include "StdAfx.h"
#include "SFSystem.h"
#include "SFResMgr.h"
#include "SFRes.h"
#include "SFScene1.h"
#include "SFScene2.h"
#include "SDKSound.h"
#include "DXUT.h"
#include "ScreenMgr.h"

CSound*        g_pSound=NULL;
CSound*        g_pSoundFire=NULL;
ID3DXEffect*		g_pEffect = NULL;

SFSystem::SFSystem(void)
: m_pScene(NULL)
{
}

SFSystem::~SFSystem(void)
{
}

BOOL SFSystem::InitEnvironment()
{
	m_pResMgr = new SFResMgr();
	m_pResMgr->AddResource(STAR_SAIL);
	m_pResMgr->AddResource(EXPLSION_EFFECT_1);
	m_pResMgr->AddResource(HERO_PLANE);
	m_pResMgr->AddResource(EXPLSION_EFFECT_2);
	m_pResMgr->AddResource(EXPLSION_EFFECT_3);
	m_pResMgr->AddResource(MISAILE_1);
	m_pResMgr->AddResource(MISAILE_2);
	m_pResMgr->AddResource(PLANE_1);

	//m_pScene = new SFScene1();
	//m_pScene->Init();

	D3DXFONT_DESC desc;
	desc.CharSet = HANGEUL_CHARSET;
	desc.Height = 15;
	desc.Width = 8;
	desc.Weight = 100;
	desc.Quality = ANTIALIASED_QUALITY;
	desc.MipLevels = 1;
	desc.Italic = 0;
	desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	desc.PitchAndFamily = FF_DONTCARE;
	_tcscpy_s(desc.FaceName, _T("Arial"));

	m_Font.Initialize(&desc);

	m_pSoundManager = new CSoundManager();
	m_pSoundManager->Initialize(DXUTGetHWND(), DSSCL_PRIORITY);

	m_pSoundManager->Create( &g_pSound, L"./trun.wav" , 0, GUID_NULL );
	m_pSoundManager->Create( &g_pSoundFire, L"./fire.wav" , 0, GUID_NULL );
	
	g_pSound->Play( 0, DSBPLAY_LOOPING);

	GetD3DDevice()->GetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pGameStateMachine = new SFGameStateMachine();

	m_pGameStateMachine->AddState(SF_GAMESTATE_INIT);
	m_pGameStateMachine->AddState(SF_GAMESTATE_INTRO);
	m_pGameStateMachine->AddState(SF_GAMESTATE_PLAY);

	m_pGameStateMachine->ChangeState(SF_GAMESTATE_PLAY);


	/////////////////////////////////
	//20120205
	/////////////////////////////////
	m_pScreenMgr = new CScreenMgr();
	m_pScreenMgr->AddTextureFile(L"1_1.bmp");
	m_pScreenMgr->AddTextureFile(L"1_2.jpg");
	
	m_pScreenMgr->ResetDevice(NULL);

	ID3DXBuffer *pErrorMsgs = NULL;
	HRESULT hRes = D3DXCreateEffectFromFile(GetD3DDevice(), L"lighting.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL, &g_pEffect, &pErrorMsgs);

	if(FAILED(hRes) && (pErrorMsgs != NULL))		//Failed to create Effect
	{
		return FALSE;
	}

	return TRUE;
}

BOOL SFSystem::OnRender( float fElapsedTime )
{
	m_pGameStateMachine->OnRender(fElapsedTime);

	return TRUE;
}

void SFSystem::OnMove( double fTime, float fElapsedTime )
{ 
	m_pGameStateMachine->OnUpdate(fTime, fElapsedTime);
}

void SFSystem::LostDevice()
{

}

BOOL SFSystem::ProcessMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return TRUE;
}

BOOL SFSystem::ResetDevice( IDirect3DDevice9* pd3dDevice )
{	
	SetD3DDevice(pd3dDevice);

	if(m_pScene)
	m_pScene->ResetDevice(pd3dDevice);
	return TRUE;
}