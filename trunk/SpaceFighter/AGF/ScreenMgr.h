#pragma once

#define MAX_IMAGE_COUNT 10
class CScreenMgr
{
public:
	CScreenMgr(void);
	virtual ~CScreenMgr(void);

	BOOL DoAlphaBlending(float fElapsedTime);
	BOOL FadeQuadVerts(D3DXCOLOR FadeToColor, float fFadeTime, float fEffectTime, LPDIRECT3DVERTEXBUFFER9 pVB);
	BOOL ResetDevice();

	BOOL Start(TCHAR* szGameName);
	BOOL GetAlphaBleningImageList(std::wstring* aImageName, int iBlendingImageMax, TCHAR* szGameName, int& iImageCount);

	BOOL GetEnable(){return m_bEnable;}
	void SetEnable(BOOL bEnable){m_bEnable = bEnable;}

	BOOL AddTextureFile(TCHAR* szTextureName);
	BOOL ResetDevice(TCHAR* szFilePath);

	BOOL DoIntro(float fElapsedTime);
	BOOL FadeEffect(float fElapsedTime, int iBlackImage, int iImage);

	BOOL GetIntroEnd(){return m_bIntroEnd;}

protected:

private:
	BOOL m_bEnable;

	int m_iCount;
	float m_fTempBleningTime;
	float m_fBleningTime;

	int m_SelectedImageIndex1;
	int m_SelectedImageIndex2;

	D3DXCOLOR m_FadeToColor;

	LPDIRECT3DTEXTURE9 m_aTexture[MAX_IMAGE_COUNT];
	LPDIRECT3DVERTEXBUFFER9  m_aVertexBuffer[MAX_IMAGE_COUNT];
	TCHAR m_szGameName[MAX_PATH];

	std::wstring m_aImageName[MAX_IMAGE_COUNT];

	int m_iBlackImage;
	int m_SelectedImageIndex;
	BOOL m_bFadeIn;
	BOOL m_bIntroEnd;
};