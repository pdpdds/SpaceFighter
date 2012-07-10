#include "StdAfx.h"
#include "dxstdafx.h"
#include "DirectXSystem.h"
#include "DirectInput.h"
#include "PannelManager.h"
#include "lua.hpp"
#include "LuaBindPanel.h"
//#include <atlbase.h>
#include "PictureBox.h"
#include "Pannel.h"
#include "ScreenMgr.h"
#include "Sample.h"
#include "CommDlg.h"
#include "BassSoundManager.h"

#include "d3dUtility.h"
#include "psystem.h"
#include "camera.h"
#include <cstdlib>
#include <ctime>

extern void RegisterAllLuaBinds(lua_State*);
Sample sample;

CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CModelViewerCamera      g_Camera;
D3DXMATRIXA16           g_mView;
D3DXMATRIXA16           g_mWorld;

ID3DXFont*                  g_pFont2 = NULL;
ID3DXSprite*                g_pTextSprite = NULL;   // Sprite for batching draw text calls

psys::PSystem* Sno = 0;

Camera TheCamera(Camera::AIRCRAFT);

#pragma comment(lib, "dinput8.lib")

//
// Framework Functions
//
bool Setup()
{
	// seed random number generator
	srand((unsigned int)time(0));

	//
	// Create Snow System.
	//

	d3d::BoundingBox boundingBox;
	boundingBox._min = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);
	boundingBox._max = D3DXVECTOR3( 10.0f,  10.0f,  10.0f);

	//D3DXVECTOR3 origin(0.0f, 10.0f, 50.0f);
	//Sno = new psys::Firework(&origin, 6000);
	//Sno->init(CDirectXSystem::GetInstance()->GetD3DDevice(), L"./res/flare.bmp");

	Sno = new psys::ParticleGun( &TheCamera );
	Sno->init(CDirectXSystem::GetInstance()->GetD3DDevice(), L"./res/flare_alpha.dds");

	//Sno = new psys::Snow(&boundingBox, 5000);
	//Sno->init(CDirectXSystem::GetInstance()->GetD3DDevice(), L"./res/snowflake.dds");

	//
	// Create basic scene.
	//
	//d3d::DrawBasicScene(CDirectXSystem::GetInstance()->GetD3DDevice(), 1.0f);

	//
	// Set projection matrix.
	//
	/*D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI / 4.0f, // 45 - degree
		(float)Width / (float)Height,
		1.0f,
		5000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);*/

	return true;
}

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    // Must support pixel shader 2.0
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	assert( DXUT_D3D9_DEVICE == pDeviceSettings->ver );

    HRESULT hr;
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    D3DCAPS9 caps;

    V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal,
                            pDeviceSettings->d3d9.DeviceType,
                            &caps ) );

    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
        caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
    {
        pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
        pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext )
{
    HRESULT hr;

    CDXUTIMEEditBox::Initialize( DXUTGetHWND() );

    V_RETURN( g_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
    // Initialize the font
   /* V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                              L"Arial", &g_pFont ) );

    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
#if defined( DEBUG ) || defined( _DEBUG )
        dwShaderFlags |= D3DXSHADER_DEBUG;
    #endif
#ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
#ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    // Read the D3DX effect file
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"CustomUI.fx" ) );
    V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
                                        NULL, &g_pEffect, NULL ) );

    g_Mesh.Create( pd3dDevice, L"misc\\cell.x" );*/
	if( FAILED( hr = D3DXCreateFont( pd3dDevice, -12, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"HY°ß°íµñ º¸Åë", &g_pFont2 ) ) )
		return hr;

	if( FAILED( hr = D3DXCreateSprite( pd3dDevice, &g_pTextSprite ) ) )
		return hr;

    // Setup the camera's view parameters
    D3DXVECTOR3 vecEye( 0.0f, 1.5f, -7.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 0.2f, 0.0f );
    D3DXVECTOR3 vecUp ( 0.0f, 1.0f, 0.0f );
    g_Camera.SetViewParams( &vecEye, &vecAt );
    D3DXMatrixLookAtLH( &g_mView, &vecEye, &vecAt, &vecUp );

	sample.Init(pd3dDevice);

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice,
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	D3DXMATRIX  matView;

	D3DXVECTOR3 vEye = D3DXVECTOR3( 0.0f, -0.0f, -1.0f );
	D3DXVECTOR3 vAt  = D3DXVECTOR3( 0.0f, -0.0f, 0.0f ); 
	D3DXVECTOR3 vUp  = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );  
	D3DXMatrixLookAtLH( &matView, &vEye, &vAt, &vUp );  
	pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	pd3dDevice->SetTransform( D3DTS_WORLD, &g_mWorld );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );  
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );

	CDirectXSystem::GetInstance()->SetWidth(pBackBufferSurfaceDesc->Width);
	CDirectXSystem::GetInstance()->SetHeight(pBackBufferSurfaceDesc->Height);

	CDirectXSystem::GetInstance()->ResetDevice(pd3dDevice);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	CDirectXSystem::GetInstance()->OnMove(fTime, fElapsedTime);
}

//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{	
	g_DialogResourceManager.OnD3D9LostDevice();

	CDirectXSystem::GetInstance()->LostDevice();
}

//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
	CDXUTIMEEditBox::Uninitialize( );

	g_DialogResourceManager.OnD3D9DestroyDevice();
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
	*pbNoFurtherProcessing = CDXUTIMEEditBox::StaticMsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

	 *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
	
	CDirectXSystem::GetInstance()->ProcessMessage( hWnd, uMsg, wParam, lParam );
	
	// Pass all remaining windows messages to camera so it can respond to user input
	g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

	return 0;
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;
	D3DXMATRIXA16 mWorldViewProjection;    
	D3DXMATRIX  g_matView;

	D3DXVECTOR3 vEye = D3DXVECTOR3( 0.0f, -0.0f, -1.0f );
	D3DXVECTOR3 vAt  = D3DXVECTOR3( 0.0f, -0.0f, 0.0f ); 
	D3DXVECTOR3 vUp  = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );  
	D3DXMatrixLookAtLH( &g_matView, &vEye, &vAt, &vUp );  
	pd3dDevice->SetTransform( D3DTS_VIEW, &g_matView );


	// Clear the render target and the zbuffer 
	V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0) );
				
	CDirectXSystem::GetInstance()->OnRender(fElapsedTime);

	pd3dDevice->Present(NULL,NULL,NULL,NULL);
}

//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}


CDirectXSystem* CDirectXSystem::m_pDirectXSystem = NULL;

CDirectXSystem::CDirectXSystem(void)
{
	m_pPannelManager = NULL;
	m_pMainCursor = NULL;
}

CDirectXSystem::~CDirectXSystem(void)
{
	if(m_pDirectXSystem)
		delete m_pDirectXSystem;
}

BOOL CDirectXSystem::Initialize()
{
	// Set DXUT callbacks
    DXUTSetCallbackD3D9DeviceAcceptable( IsDeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnCreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnFrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnLostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( KeyboardProc );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

	// Show the cursor and clip it when in full screen
	DXUTSetCursorSettings( false, false );

	//m_pBassSoundManager = NULL;
	//m_pBassSoundManager = new CBassSoundManager();
	//m_pBassSoundManager->Initialize();

	return TRUE;
}

int CDirectXSystem::Run(int Width, int Height, TCHAR* szGameTitle)
{
	// Initialize the sample framework and create the desired Win32 window and Direct3D 
	// device for the application. Calling each of these functions is optional, but they
	// allow you to set several options which control the behavior of the framework.
	DXUTInit( true, false, false ); // Parse the command line, handle the default hotkeys, and show msgboxes

	m_iWidth = Width;
	m_iHeight = Height;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////////////////////

	DXUTSetHotkeyHandling( true, true, true );
    DXUTCreateWindow(szGameTitle );
    DXUTCreateDevice( true, m_iWidth, m_iHeight );

	m_pInputSystem = new DirectInput( ((HINSTANCE)::GetWindowLong((DXUTGetHWND()), GWL_HINSTANCE)) , DXUTGetHWND(), DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);

	InitEnvironment();

	m_pd3dDevice->CreateStateBlock( D3DSBT_ALL, &m_pInitStateBlock ); 
	
	// Pass control to the sample framework for handling the message pump and 
	// dispatching render calls. The sample framework will call your FrameMove 
	// and FrameRender callback when there is idle time between handling window messages.
	DXUTMainLoop();

	// Perform any application-level cleanup here. Direct3D device resources are released within the
	// appropriate callback functions and therefore don't require any cleanup code here.

	return DXUTGetExitCode();
}

HRESULT CDirectXSystem::CreateTexture(LPDIRECT3DTEXTURE9 &Test_Texture, LPCTSTR szTextureName)
{
	HRESULT hr;
	if (FAILED(hr = D3DXCreateTextureFromFileEx(m_pd3dDevice, szTextureName, 
	0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
	0, NULL, NULL, &Test_Texture))) {
	return(hr);

	}

	/*D3DXCreateTextureFromFile(
		m_pd3dDevice,
		wFileName,
		&Test_Texture);  	*/

	return hr;
}

HRESULT CDirectXSystem::Create3DSprite(LPDIRECT3DTEXTURE9 &Test_Texture,LPDIRECT3DVERTEXBUFFER9 &Test_Buffer,LPCTSTR lpstr,int alpha)
{
	 IDirect3DDevice9* pd3dDevice = m_pd3dDevice;
	TCHAR szFilename[MAX_PATH];
	WCHAR wFileName[MAX_PATH];
	HRESULT hr;
	if(Test_Buffer==NULL)
	{ 
		pd3dDevice->CreateVertexBuffer(
			6 * sizeof(CUSTOMVERTEX), 
			D3DUSAGE_WRITEONLY,
			D3DFVF_CUSTOMVERTEX,
			D3DPOOL_MANAGED,
			&Test_Buffer,
			0);
	}

	CUSTOMVERTEX* v;	
	if( FAILED( hr = Test_Buffer->Lock( 0, 0, (void **)&v, 0 ) ) )
		return hr;

	// first triangle
	v[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	v[0].color    = D3DCOLOR_ARGB(alpha,0,0,0);
	v[0].tu       = 0.0f;
	v[0].tv       = 0.0f;

	v[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	v[1].color    = D3DCOLOR_ARGB(alpha,0,0,0);
	v[1].tu       = 1.0f;
	v[1].tv       = 0.0f;

	v[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	v[2].color    = D3DCOLOR_ARGB(alpha,0,0,0);
	v[2].tu       = 1.0f;
	v[2].tv       = 1.0f;

	// second triangle
	v[3].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	v[3].color    = D3DCOLOR_ARGB(alpha,0,0,0);
	v[3].tu       = 0.0f;
	v[3].tv       = 0.0f;

	v[4].position = D3DXVECTOR3(1.0f, -1.0, 0.0f);
	v[4].color    = D3DCOLOR_ARGB(alpha,0,0,0);
	v[4].tu       = 1.0f;
	v[4].tv       = 1.0f;

	v[5].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	v[5].color    = D3DCOLOR_ARGB(alpha,0,0,0);
	v[5].tu       = 0.0f;
	v[5].tv       = 1.0f;

	if( FAILED( hr = Test_Buffer->Unlock() ) )	
		return hr;


	/*(void)StringCchCopy(szFilename, sizeof(szFilename),lpstr);
	USES_CONVERSION;

	(void)StringCchCopyW(wFileName, MAX_PATH, T2W(szFilename));*/

	/*if (FAILED(hr = D3DXCreateTextureFromFileEx(m_pd3dDevice, wFileName, 
	0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
	0, NULL, NULL, &Test_Texture))) {
	return(hr);

	}*/

	D3DXCreateTextureFromFile(
		pd3dDevice,
		lpstr,
		&Test_Texture);  	

	return hr;
}