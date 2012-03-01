#include "StdAfx.h"
#include "SFBackGroundTerrain.h"
#include "SFSystem.h"
#include "Terrain.h"
#include "SFResMgr.h"
#include "SFRes.h"
#include "Terrain.h"
#include "SFCamera.h"

SFBackGroundTerrain::SFBackGroundTerrain(void)
{
}

SFBackGroundTerrain::~SFBackGroundTerrain(void)
{
}

BOOL SFBackGroundTerrain::OnRender( float fElapsedTime )
{
	D3DXMATRIX matWorld;
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();

	static float fTerrainSprite = 0.0f;

	if( SUCCEEDED( gd3dDevice->BeginScene() ) )
	{		
		gd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);

		D3DXMatrixIdentity( &matWorld );
		gd3dDevice->SetTransform( D3DTS_VIEW, &m_pCamera->GetViewMatrix());

		D3DXMatrixTranslation(&matWorld,0, 0, fTerrainSprite * 40 + 600);
		gd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pTerrain->Render();
		D3DXMatrixTranslation(&matWorld,0, 0, fTerrainSprite * 40 + 1400.0f);
		gd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pTerrain->Render();

		fTerrainSprite -= fElapsedTime;
		if(fTerrainSprite * 40 < -800)
			fTerrainSprite = 0.0f;

		gd3dDevice->EndScene();
	}

	return TRUE;
}

BOOL SFBackGroundTerrain::OnResetDevice()
{
	return TRUE;
}

BOOL SFBackGroundTerrain::OnFrameMove( double fTime, float fElapsedTime )
{	
	return TRUE;
}

BOOL SFBackGroundTerrain::OnLostDevice()
{
	return TRUE;
}

BOOL SFBackGroundTerrain::OnDestroyDevice()
{
	return TRUE;
}

BOOL SFBackGroundTerrain::Init()
{
	m_pCamera = new CUserControlledCamera();
	m_pCamera->SetPosition(D3DXVECTOR3(0.0f, 80.0f, 0.0f));
	//m_pCamera->Update(0);

	m_pCamera->Rotation(0, PI / 2.0f ,0); 

	m_pTerrain = new CTerrain(gd3dDevice, 40, 30.0, 20.0, 40);
	m_pTerrain->SetTexture(_T("caust07.tga"));

	D3DLIGHT9 light, light2;
	memset(&light, 0, sizeof(light));
	light.Ambient.a = 1.0f; light.Ambient.g = light.Ambient.b = light.Ambient.r = 1.0f;
	light.Diffuse.a = light.Diffuse.g = light.Diffuse.b = light.Diffuse.r =1.0f;
	light.Position = D3DXVECTOR3(0.0,80.0,0.0);
	light.Direction = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	//light.Type = D3DLIGHT_DIRECTIONAL;
	light.Type = D3DLIGHT_DIRECTIONAL;

	light2 = light;
	light2.Direction = D3DXVECTOR3(-1.0f,-1.0f,0.0f);

	gd3dDevice->SetLight(0, &light);
	gd3dDevice->LightEnable(0, TRUE);

	gd3dDevice->SetLight(1, &light2);
	gd3dDevice->LightEnable(1, TRUE);

	return TRUE;
}