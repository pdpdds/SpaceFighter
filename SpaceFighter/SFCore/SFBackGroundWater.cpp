#include "StdAfx.h"
#include "SFBackGroundWater.h"
#include "SFSystem.h"
#include "Camera2.h"
#include "SFCamera.h"
#include "PondWater.h"
#include "Vertex.h"
#include "SFRes.h"
#include "SFResMgr.h"

SFBackGroundWater::SFBackGroundWater(void)
: m_pWater(NULL)
, m_pCamera(NULL)
, m_bInit(FALSE)
{
}

SFBackGroundWater::~SFBackGroundWater(void)
{
}

BOOL SFBackGroundWater::OnRender( float fElapsedTime )
{

	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();

	// Reflection plane in local space.
	D3DXPLANE waterPlaneL(0.0f, -1.0f, 0.0f, 0.0f);

	// Reflection plane in world space.
	D3DXMATRIX WInvTrans;
	D3DXMatrixInverse(&WInvTrans, 0, &(mWaterWorld));
	D3DXMatrixTranspose(&WInvTrans, &WInvTrans);
	D3DXPLANE waterPlaneW;
	D3DXPlaneTransform(&waterPlaneW, &waterPlaneL, &WInvTrans);

	// Reflection plane in homogeneous clip space.
	D3DXMATRIX WVPInvTrans;
	D3DXMatrixInverse(&WVPInvTrans, 0, &(mWaterWorld*m_pCamera->viewProj()));
	D3DXMatrixTranspose(&WVPInvTrans, &WVPInvTrans);

	D3DXPLANE waterPlaneH;
	D3DXPlaneTransform(&waterPlaneH, &waterPlaneL, &WVPInvTrans);


	DrawableTex2D* reflectMap = m_pWater->mReflectMap;
	DrawableTex2D* refractMap = m_pWater->mRefractMap;


	float f[4] = {waterPlaneH.a, waterPlaneH.b, waterPlaneH.c, waterPlaneH.d};
	HR(gd3dDevice->SetClipPlane(0, (float*)f));
	HR(gd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1));
	refractMap->beginScene();

	//mSky->draw(0);
	drawSceneMesh(0);
	refractMap->endScene();

	// Seems like we need to reset these due to a driver bug.  It works
	// correctly without these next two lines in the REF and another 
	// video card, however.
	HR(gd3dDevice->SetClipPlane(0, (float*)f));
	HR(gd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1));

	HR(gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW));
	reflectMap->beginScene();
	//	mSky->draw(&waterPlaneW);
	drawSceneMesh(&waterPlaneW);
	reflectMap->endScene();

	HR(gd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 0));
	HR(gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW));

	HR(gd3dDevice->BeginScene());

	//	mSky->draw(0);
	drawSceneMesh(0);
	m_pWater->draw();

	//mGfxStats->display();

	HR(gd3dDevice->EndScene());

	HR(gd3dDevice->BeginScene());

	//	mSky->draw(0);
	drawSceneMesh(0);
	m_pWater->draw();

	//	mGfxStats->display();

	HR(gd3dDevice->EndScene());

	HR(gd3dDevice->BeginScene());

	//m_pMyScene->VirtualRender();
	
	HR(gd3dDevice->EndScene());

	
	gd3dDevice->SetTransform( D3DTS_VIEW, &m_pCamera2->GetViewMatrix() );  

	//g_HUD.OnRender( 10 );
	//g_SampleUI.OnRender( 10 );

	// Present the backbuffer.

	return TRUE;
}

BOOL SFBackGroundWater::Init()
{
	m_pCamera2 = new CUserControlledCamera();
	m_pCamera2->SetPosition(D3DXVECTOR3(0.0f, 80.0f, 0.0f));
	//m_pCamera->Update(0);

	m_pCamera2->Rotation(0, PI / 2.0f ,0); 

	m_pCamera = new Camera2();

	// Initialize camera.
	m_pCamera->pos().y = 50.0f;
	m_pCamera->pos().z = -30.0f;
	m_pCamera->setSpeed(1.0f);

	InitAllVertexDeclarations();

	mLight.dirW = D3DXVECTOR3(0.0f, -1.0f, -3.0f);
	D3DXVec3Normalize(&mLight.dirW, &mLight.dirW);
	mLight.ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mLight.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mLight.spec    = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	Mtrl waterMtrl;
	waterMtrl.ambient   = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.00f);
	waterMtrl.diffuse   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.00f);
	waterMtrl.spec      = 0.6f*WHITE;
	waterMtrl.specPower = 200.0f;

	D3DXMatrixTranslation(&mWaterWorld, 0.0f, 2.0f, 0.0f);

	PondWater::InitInfo waterInitInfo;
	waterInitInfo.dirLight = mLight;
	waterInitInfo.mtrl     = waterMtrl;
	waterInitInfo.vertRows         = 128;
	waterInitInfo.vertCols         = 128;
	waterInitInfo.dx               = 1.0f;
	waterInitInfo.dz               = 1.0f;
	waterInitInfo.waveMapFilename0 = _T("wave0.dds");
	waterInitInfo.waveMapFilename1 = _T("wave1.dds");
	waterInitInfo.waveMapVelocity0 = D3DXVECTOR2(0.09f, 0.06f);
	waterInitInfo.waveMapVelocity1 = D3DXVECTOR2(-0.05f, 0.08f);
	waterInitInfo.texScale = 10.0f; 
	waterInitInfo.refractBias = 0.1f;
	waterInitInfo.refractPower = 2.0f;
	waterInitInfo.rippleScale  = D3DXVECTOR2(0.06f, 0.03f); 
	waterInitInfo.toWorld = mWaterWorld;

	D3DXMatrixIdentity(&mSceneWorld);
	D3DXMatrixIdentity(&mSceneWorldInv);

	HR(D3DXCreateTextureFromFile(gd3dDevice, _T("floor_nmap.bmp"), &mSceneNormalMaps[0]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, _T("bricks_nmap.bmp"), &mSceneNormalMaps[1]));

	HR(D3DXCreateTextureFromFile(gd3dDevice, _T("whitetex.dds"), &mWhiteTex));

	ID3DXMesh* tempMesh = 0;
	LoadXFile(_T("BasicColumnScene.x"), &tempMesh, mSceneMtrls, mSceneTextures);

	// Get the vertex declaration for the NMapVertex.
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	HR(NMapVertex::Decl->GetDeclaration(elems, &numElems));

	// Clone the mesh to the NMapVertex format.
	ID3DXMesh* clonedTempMesh = 0;
	HR(tempMesh->CloneMesh(D3DXMESH_MANAGED, elems, gd3dDevice, &clonedTempMesh));

	// Now use D3DXComputeTangentFrameEx to build the TNB-basis for each vertex
	// in the mesh.  

	HR(D3DXComputeTangentFrameEx(
		clonedTempMesh, // Input mesh
		D3DDECLUSAGE_TEXCOORD, 0, // Vertex element of input tex-coords.  
		D3DDECLUSAGE_BINORMAL, 0, // Vertex element to output binormal.
		D3DDECLUSAGE_TANGENT, 0,  // Vertex element to output tangent.
		D3DDECLUSAGE_NORMAL, 0,   // Vertex element to output normal.
		0, // Options
		0, // Adjacency
		0.01f, 0.25f, 0.01f, // Thresholds for handling errors
		&mSceneMesh, // Output mesh
		0));         // Vertex Remapping

	// Done with temps.
	ReleaseCOM(tempMesh);
	ReleaseCOM(clonedTempMesh);

	D3DXMatrixIdentity(&mSceneWorld);
	D3DXMatrixIdentity(&mSceneWorldInv);

	m_pWater = new PondWater(waterInitInfo, m_pCamera);

	buildFX();
	HR(mFX->OnResetDevice());
	m_pWater->onResetDevice();

	float fWidth = (float)CDirectXSystem::GetInstance()->GetWidth();
	float fHeight = (float)CDirectXSystem::GetInstance()->GetHeight();
	m_pCamera->setLens(D3DX_PI * 0.25f, fWidth/fHeight, 1.0f, 15000.0f);

	m_pCamera->update2(0,0,0);

	m_bInit = TRUE;

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

BOOL SFBackGroundWater::OnFrameMove( double fTime, float fElapsedTime )
{
	if(m_bInit)
	{
		D3DXVECTOR3 pos = m_pCamera->pos();
		pos.z +=  fElapsedTime * 1;

		D3DXVECTOR3 pos2 = pos;
		pos2.y = -10.0f;
		m_pCamera->lookAt(pos, pos2, D3DXVECTOR3(0,0,1));
		//m_pCamera->update(fElapsedTime,0,0);
		m_pWater->update(fElapsedTime);

	}

	return TRUE;

}

BOOL SFBackGroundWater::OnResetDevice()
{
	float fWidth = (float)CDirectXSystem::GetInstance()->GetWidth();
	float fHeight = (float)CDirectXSystem::GetInstance()->GetHeight();
	m_pCamera->setLens(D3DX_PI * 0.25f, fWidth/fHeight, 1.0f, 15000.0f);

	return TRUE;
}

void SFBackGroundWater::drawSceneMesh(D3DXPLANE* reflectedW)
{
	D3DXMATRIX RefMtx;
	D3DXMatrixIdentity(&RefMtx);
	if(reflectedW)
		D3DXMatrixReflect(&RefMtx, reflectedW);


	HR(mFX->SetValue(mhLight, &mLight, sizeof(DirLight)));
	HR(mFX->SetMatrix(mhWVP, &(mSceneWorld*RefMtx*m_pCamera->viewProj())));
	HR(mFX->SetValue(mhEyePosW, &m_pCamera->pos(), sizeof(D3DXVECTOR3)));

	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	HR(mFX->BeginPass(0));

	for(UINT j = 0; j < mSceneMtrls.size(); ++j)
	{
		HR(mFX->SetValue(mhMtrl, &mSceneMtrls[j], sizeof(Mtrl)));

		// If there is a texture, then use.
		if(mSceneTextures[j] != 0)
		{
			HR(mFX->SetTexture(mhTex, mSceneTextures[j]));
		}

		// But if not, then set a pure white texture.  When the texture color
		// is multiplied by the color from lighting, it is like multiplying by
		// 1 and won't change the color from lighting.
		else
		{
			HR(mFX->SetTexture(mhTex, mWhiteTex));
		}

		HR(mFX->SetTexture(mhNormalMap, mSceneNormalMaps[j]));

		HR(mFX->CommitChanges());
		HR(mSceneMesh->DrawSubset(j));
	}
	HR(mFX->EndPass());
	HR(mFX->End());
}

void SFBackGroundWater::buildFX()
{
	// Create the FX from a .fx file.
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, _T("NormalMap.fx"), 
		0, 0, 0, 0, &mFX, &errors));
	if( errors )
		MessageBoxA(0, (LPCSTR)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	mhTech       = mFX->GetTechniqueByName("NormalMapTech");
	mhWVP        = mFX->GetParameterByName(0, "gWVP");
	mhWorldInv   = mFX->GetParameterByName(0, "gWorldInv");
	mhMtrl       = mFX->GetParameterByName(0, "gMtrl");
	mhLight      = mFX->GetParameterByName(0, "gLight");
	mhEyePosW    = mFX->GetParameterByName(0, "gEyePosW");
	mhTex        = mFX->GetParameterByName(0, "gTex");
	mhNormalMap  = mFX->GetParameterByName(0, "gNormalMap");

	// Set parameters that do not vary:

	// World is the identity, so inverse is also identity.
	HR(mFX->SetMatrix(mhWorldInv, &mSceneWorldInv));
	HR(mFX->SetTechnique(mhTech));
}

BOOL SFBackGroundWater::OnLostDevice()
{
	return TRUE;
}

BOOL SFBackGroundWater::OnDestroyDevice()
{
	return TRUE;
}