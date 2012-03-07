#pragma once
#include "DirectXSystem.h"
#include "SFd3dUtil.h"
#include "SFGameStateMachine.h"

class CTerrain;
class SFResMgr;
class PondWater;
class SFScene;

class SFSystem : public CDirectXSystem
{
public:
	SFSystem(void);
	virtual ~SFSystem(void);

	virtual BOOL InitEnvironment();
	virtual BOOL OnRender(float fElapsedTime);
	virtual void OnMove( double fTime, float fElapsedTime);
	virtual void LostDevice();

	virtual BOOL ProcessMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;
	virtual BOOL ResetDevice( IDirect3DDevice9* pd3dDevice) ;

	static CDirectXSystem* GetInstance()
	{
		if (NULL == m_pDirectXSystem)
		{
			m_pDirectXSystem = new SFSystem();
		}
		return m_pDirectXSystem;
	}

	SFResMgr* GetResMgr(){return m_pResMgr;}
	SFScene* GetCurrentScene(){return m_pScene;}
	void SetCurrentScene(SFScene* pScene){m_pScene = pScene;}
	CFontObject* GetDebugFont(){return &m_Font;}

	CScreenMgr* GetScreenMgr(){return m_pScreenMgr;}
	SFGameStateMachine* GetGameStateMachine(){return m_pGameStateMachine;}

	D3DXMATRIX* GetProjMatrix(){return &m_matProj;}

protected:

private:
	SFGameStateMachine* m_pGameStateMachine;

	SFResMgr* m_pResMgr;
	SFScene* m_pScene;
	CFontObject m_Font;

	CScreenMgr* m_pScreenMgr;
	D3DXMATRIX m_matProj;
};
