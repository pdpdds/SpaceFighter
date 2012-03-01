#include "StdAfx.h"
#include "ScreenMgr.h"
#include "dxstdafx.h"
#include "DirectXSystem.h"

CScreenMgr::CScreenMgr(void)
{
	m_bEnable = FALSE;
	m_SelectedImageIndex1 = 0;
	m_SelectedImageIndex2 = 1;

	memset(m_aTexture, 0, sizeof(LPDIRECT3DTEXTURE9) * 10);	
	memset(m_aVertexBuffer, 0, sizeof(LPDIRECT3DVERTEXBUFFER9) * 10);

	m_FadeToColor = D3DXCOLOR(0,0,0,0);
	m_iCount = 0;
	m_fTempBleningTime = 0.0f;
	m_fBleningTime = 5.0f;

	m_iBlackImage = 0;
	m_SelectedImageIndex = 1;
	m_bFadeIn = TRUE;
	m_bIntroEnd = FALSE;

}

CScreenMgr::~CScreenMgr(void)
{
}

BOOL CScreenMgr::DoAlphaBlending(float fElapsedTime)
{

	IDirect3DDevice9* pd3dDevice = CDirectXSystem::GetInstance()->GetD3DDevice();
	int iWidth = CDirectXSystem::GetInstance()->GetWidth();
	int iHeight = CDirectXSystem::GetInstance()->GetHeight();

	if(NULL == pd3dDevice)
		return FALSE;

	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{


		D3DXMATRIX mat;  		
		D3DXMatrixOrthoLH(&mat, (float)iWidth, (float)iHeight,     			
			0.0, 1000.0);  		
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat );    		
		D3DXMATRIX matWorld,matTrans,matScale;  		
		D3DXMatrixScaling(&matScale, (float)iWidth/2.0f, (float)iHeight/2.0f, 1.0); 		
		D3DXMatrixMultiply(&matWorld, &matScale, D3DXMatrixTranslation(&matTrans, -0.5f, -0.5f, 0));  		
		pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );  

		pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		
		pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		
		pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);		
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);     		
		pd3dDevice->SetTexture( 0, m_aTexture[m_SelectedImageIndex1] );		   		
		pd3dDevice->SetStreamSource( 0, m_aVertexBuffer[m_SelectedImageIndex1],0, sizeof(CUSTOMVERTEX) );		
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );    		
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );    		
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );    		
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );												
		pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );		
		pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );	

		m_fTempBleningTime += fElapsedTime;

		if(m_fTempBleningTime > m_fBleningTime && m_iCount > 1)		
		{		
			pd3dDevice->SetTexture( 0, m_aTexture[m_SelectedImageIndex2] );		
			pd3dDevice->SetStreamSource( 0, m_aVertexBuffer[m_SelectedImageIndex2],0, sizeof(CUSTOMVERTEX) );    		

			// set up texture stage states for blending the 2nd quad on top    		
			//pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );		
			// put the correct colors into the quad's verts      		
			FadeQuadVerts(m_FadeToColor,fElapsedTime, 2.0f, m_aVertexBuffer[m_SelectedImageIndex2]); 

			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );      		
			pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );      		
			pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );      		
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );      		
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

			if(m_FadeToColor.a >= 1)  
			{
				m_fTempBleningTime = 0;
				m_SelectedImageIndex1 = m_SelectedImageIndex2;
				m_SelectedImageIndex2 = m_SelectedImageIndex2++;

				if(m_SelectedImageIndex2 >= m_iCount)
					m_SelectedImageIndex2 = 0;

				m_FadeToColor.a=0;	   
			}
		}

		pd3dDevice->EndScene();

	}

	return TRUE;
}

BOOL CScreenMgr::FadeQuadVerts(D3DXCOLOR FadeToColor, float fFadeTime, 
				   float fEffectTime, LPDIRECT3DVERTEXBUFFER9 pVB)
{
	HRESULT hr;
	CUSTOMVERTEX* pVertices;  

	// got it got it got it got it  
	if( FAILED( hr = pVB->Lock( 0, 6*sizeof(CUSTOMVERTEX), (void**)&pVertices, 0 ) ) )
		return false;

	// calculate the new color value

	m_FadeToColor.a+=(fFadeTime / fEffectTime);  
	if (m_FadeToColor.a >= 255) 
		m_FadeToColor.a = 255;  

	pVertices[0].color    = (DWORD)m_FadeToColor;  
	pVertices[1].color    = (DWORD)m_FadeToColor;  
	pVertices[2].color    = (DWORD)m_FadeToColor;  
	pVertices[3].color    = (DWORD)m_FadeToColor;  
	pVertices[4].color    = (DWORD)m_FadeToColor;  
	pVertices[5].color    = (DWORD)m_FadeToColor;

	if( FAILED( hr = pVB->Unlock() ) ) return FALSE;
	return TRUE;

	return FALSE;
}

BOOL CScreenMgr::ResetDevice()
{
	for(int i = 0; i < MAX_IMAGE_COUNT; i++)
	{
		SAFE_RELEASE(m_aTexture[i]);	
		SAFE_RELEASE(m_aVertexBuffer[i]);
	}

	m_SelectedImageIndex1 = 0;
	m_SelectedImageIndex2 = 1;

	IDirect3DDevice9* pd3dDevice = CDirectXSystem::GetInstance()->GetD3DDevice();
	int iWidth = CDirectXSystem::GetInstance()->GetWidth();
	int iHeight = CDirectXSystem::GetInstance()->GetHeight();

	int iCount = 0;

	while (iCount < m_iCount)
	{			
		WCHAR wFileName[MAX_PATH];
		StringCchPrintf( wFileName, MAX_PATH, L"./GameList/%s/%s", m_szGameName, m_aImageName[iCount].c_str());
		CDirectXSystem::GetInstance()->Create3DSprite(m_aTexture[iCount],m_aVertexBuffer[iCount], wFileName,0);	
		iCount++;
	}

	return TRUE;
}

BOOL CScreenMgr::GetAlphaBleningImageList(std::wstring* aImageName, int iBlendingImageMax, TCHAR* szGameName, int& iImageCount)
{
	HANDLE File_Hwnd;	
	WIN32_FIND_DATA wfd;

	BOOL bResult = TRUE;

	WCHAR wFileName[MAX_PATH];
	StringCchPrintf( wFileName, MAX_PATH, L"./GameList/%s/*.jpg", szGameName);

	File_Hwnd=FindFirstFile(wFileName,&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult = FALSE;
	else bResult = TRUE;

	while (bResult)
	{

		//_tcscpy(aImageName[iIndex], wfd.cFileName);
		aImageName[iImageCount] =  wfd.cFileName;

		iImageCount++;

		if(iImageCount >= iBlendingImageMax)
			break;

		bResult = FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);

	return TRUE;
}

BOOL CScreenMgr::Start(TCHAR* szGameName)
{
	SetEnable(TRUE);

	m_iCount = 0;
	m_fTempBleningTime = 0.0f;
	m_fBleningTime = 5.0f;

	GetAlphaBleningImageList(m_aImageName, MAX_IMAGE_COUNT, szGameName, m_iCount);
	_tcscpy_s(m_szGameName, szGameName);

	ResetDevice();

	return TRUE;
}

BOOL CScreenMgr::AddTextureFile(TCHAR* szTextureName)
{
	if(m_iCount >= MAX_IMAGE_COUNT)
		return FALSE;

	m_aImageName[m_iCount] = szTextureName;
	
	//_tcscpy(m_aImageName[m_iCount], szTextureName);

	m_iCount++;

	return TRUE;
}

BOOL CScreenMgr::ResetDevice(TCHAR* szFilePath)
{
	for(int i = 0; i < MAX_IMAGE_COUNT; i++)
	{
		SAFE_RELEASE(m_aTexture[i]);	
		SAFE_RELEASE(m_aVertexBuffer[i]);
	}

	m_SelectedImageIndex1 = 0;
	m_SelectedImageIndex2 = 1;

	IDirect3DDevice9* pd3dDevice = CDirectXSystem::GetInstance()->GetD3DDevice();
	int iWidth = CDirectXSystem::GetInstance()->GetWidth();
	int iHeight = CDirectXSystem::GetInstance()->GetHeight();

	int iCount = 0;

	while (iCount < m_iCount)
	{			
		WCHAR wFileName[MAX_PATH];
		//StringCchPrintf( wFileName, MAX_PATH, L"./GameList/%s/%s", m_szGameName, m_aImageName[iCount].c_str());
		CDirectXSystem::GetInstance()->Create3DSprite(m_aTexture[iCount],m_aVertexBuffer[iCount], m_aImageName[iCount].c_str(),0);	
		iCount++;
	}

	return TRUE;
}

BOOL CScreenMgr::FadeEffect(float fElapsedTime, int iBlackImage, int iImage)
{
	IDirect3DDevice9* pd3dDevice = CDirectXSystem::GetInstance()->GetD3DDevice();
	int iWidth = CDirectXSystem::GetInstance()->GetWidth();
	int iHeight = CDirectXSystem::GetInstance()->GetHeight();

	D3DXMATRIX mat;  		
	D3DXMatrixOrthoLH(&mat, (float)iWidth, (float)iHeight,     			
		0.0, 1000.0);  		
	pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat );    		
	D3DXMATRIX matWorld,matTrans,matScale;  		
	D3DXMatrixScaling(&matScale, (float)iWidth/2.0f, (float)iHeight/2.0f, 1.0); 		
	D3DXMatrixMultiply(&matWorld, &matScale, D3DXMatrixTranslation(&matTrans, -0.5f, -0.5f, 0));  		
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );  

	pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		
	pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		
	pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);		
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);     		
	pd3dDevice->SetTexture( 0, m_aTexture[iBlackImage] );		   		
	pd3dDevice->SetStreamSource( 0, m_aVertexBuffer[iBlackImage],0, sizeof(CUSTOMVERTEX) );		
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );    		
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );    		
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );    		
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );												
	pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );		
	pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );	

	m_fTempBleningTime += fElapsedTime;

	if(m_fTempBleningTime > m_fBleningTime)
	{
		pd3dDevice->SetTexture( 0, m_aTexture[iImage] );		
		pd3dDevice->SetStreamSource( 0, m_aVertexBuffer[iImage],0, sizeof(CUSTOMVERTEX) );    		

		// set up texture stage states for blending the 2nd quad on top    		
		//pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );		
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );		
		// put the correct colors into the quad's verts      		
		FadeQuadVerts(m_FadeToColor,fElapsedTime, 2.0f, m_aVertexBuffer[iImage]); 

		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );      		
		pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );      		
		pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );      		
		pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );      		
		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	}

	return FALSE;
}

BOOL CScreenMgr::DoIntro(float fElapsedTime)
{
	if(m_bIntroEnd == TRUE)
		return FALSE;

	IDirect3DDevice9* pd3dDevice = CDirectXSystem::GetInstance()->GetD3DDevice();
	int iWidth = CDirectXSystem::GetInstance()->GetWidth();
	int iHeight = CDirectXSystem::GetInstance()->GetHeight();

	if(NULL == pd3dDevice)
		return FALSE;

	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		if(TRUE == m_bFadeIn)
		{
			FadeEffect(fElapsedTime, m_iBlackImage, m_SelectedImageIndex);

			if(m_FadeToColor.a >= 1)  
			{
				m_FadeToColor.a = 0;
				m_fTempBleningTime = 0;
				m_bFadeIn = FALSE;
			}
		}
		else
		{
			FadeEffect(fElapsedTime, m_SelectedImageIndex, m_iBlackImage);
			if(m_FadeToColor.a >= 1)  
			{
				m_FadeToColor.a = 0;
				m_fTempBleningTime = 0;
				m_bFadeIn = TRUE;

				m_SelectedImageIndex++;
				if(m_SelectedImageIndex >= m_iCount)
				{
					m_bIntroEnd = TRUE;
					m_SelectedImageIndex = 0;
				}
			}
		}

		pd3dDevice->EndScene();

	}

	return TRUE;
}