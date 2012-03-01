#pragma once
#include "DirectXSystem.h"
#include "SFd3dUtil.h"
#include "SFBackGround.h"

class PondWater;
class Camera2;
class CUserControlledCamera;

class SFBackGroundWater : public SFBackGround
{
public:
	SFBackGroundWater(void);
	virtual ~SFBackGroundWater(void);

	virtual BOOL Init() override;

	virtual BOOL OnRender(float fElapsedTime) override;
	virtual BOOL OnResetDevice() override;
	virtual BOOL OnFrameMove( double fTime, float fElapsedTime) override;
	virtual BOOL OnLostDevice() override;
	virtual BOOL OnDestroyDevice() override;

	Camera2* GetCamera(){return m_pCamera;}
	void drawSceneMesh(D3DXPLANE* reflectedW);
	void buildFX();

private:
	PondWater* m_pWater;
	Camera2* m_pCamera;
	CUserControlledCamera* m_pCamera2;

	D3DXMATRIX mWaterWorld;
	DirLight mLight;

	ID3DXEffect* mFX;
	D3DXHANDLE   mhTech;
	D3DXHANDLE   mhWVP;
	D3DXHANDLE   mhWorldInv;
	D3DXHANDLE   mhEyePosW;
	D3DXHANDLE   mhTex;
	D3DXHANDLE   mhMtrl;
	D3DXHANDLE   mhLight;
	D3DXHANDLE   mhNormalMap;

	ID3DXMesh* mSceneMesh;
	D3DXMATRIX mSceneWorld;
	D3DXMATRIX mSceneWorldInv;
	std::vector<Mtrl> mSceneMtrls;
	std::vector<IDirect3DTexture9*> mSceneTextures;

	IDirect3DTexture9* mSceneNormalMaps[2];

	IDirect3DTexture9* mWhiteTex;

	BOOL m_bInit;
};
