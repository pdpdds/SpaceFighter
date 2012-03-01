#pragma once
#include "d3d9.h"
#include "TextScrollManager.h"

class CPannelManager;
class CPictureBox;
class CScreenMgr;
class DirectInput;
class CSoundManager;

class CBassSoundManager;

class CDirectXSystem
{
public:
	CDirectXSystem(void);
	virtual ~CDirectXSystem(void);

	BOOL Initialize();
	int Run(int Width, int Height, TCHAR* szGameTitle);

	virtual BOOL InitEnvironment() {return FALSE;}
	virtual BOOL OnRender(float fElapsedTime) {return FALSE;}
	virtual void OnMove( double fTime, float fElapsedTime) {}
	virtual void LostDevice() {}

	virtual BOOL ProcessMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {return FALSE;}
	virtual BOOL ResetDevice( IDirect3DDevice9* pd3dDevice) {return FALSE;}
	
	HRESULT Create3DSprite(LPDIRECT3DTEXTURE9 &Test_Texture,LPDIRECT3DVERTEXBUFFER9 &Test_Buffer,LPCTSTR lpstr, int alpha);
	int GetWidth(){return m_iWidth;}
	int GetHeight(){return m_iHeight;}

	void SetWidth(int iWidth){m_iWidth = iWidth;}
	void SetHeight(int iHeight){m_iHeight = iHeight;}

	void SetD3DDevice(IDirect3DDevice9* pd3dDevice){m_pd3dDevice = pd3dDevice;}
	IDirect3DDevice9* GetD3DDevice(){return m_pd3dDevice;}

	CPannelManager* GetPannelManager(){return m_pPannelManager;}
	CBassSoundManager* GetSoundManager(){return m_pBassSoundManager;}
	HRESULT CreateTexture(LPDIRECT3DTEXTURE9 &Test_Texture, LPCTSTR szTextureName);

	DirectInput* GetInputSystem(){return m_pInputSystem;}

	static CDirectXSystem* GetInstance()
	{
		if (NULL == m_pDirectXSystem)
		{
			m_pDirectXSystem = new CDirectXSystem();
		}
		return m_pDirectXSystem;
	}

protected:
	static CDirectXSystem* m_pDirectXSystem;
	CSoundManager*	m_pSoundManager;

private:
	CPictureBox* m_pMainCursor;
	CPannelManager* m_pPannelManager;

	IDirect3DDevice9* m_pd3dDevice;
	DirectInput* m_pInputSystem;

	int m_iWidth;
	int m_iHeight;

	// mouse button presses
	INT			m_iMouseButtons;

	// current location of Mouse
	INT			m_iMouseX;
	INT			m_iMouseY;

	CBassSoundManager* m_pBassSoundManager;
};
