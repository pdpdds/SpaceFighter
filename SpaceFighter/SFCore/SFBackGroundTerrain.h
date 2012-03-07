#pragma once
#include "SFBackGround.h"

class CTerrain;
class CUserControlledCamera;

class SFBackGroundTerrain : public SFBackGround
{
public:
	SFBackGroundTerrain(void);
	virtual ~SFBackGroundTerrain(void);

	virtual BOOL Init() override;

	virtual BOOL OnRender(float fElapsedTime) override;
	virtual BOOL OnResetDevice() override;
	virtual BOOL OnFrameMove( double fTime, float fElapsedTime) override;
	virtual BOOL OnLostDevice() override;
	virtual BOOL OnDestroyDevice() override;

	CUserControlledCamera* GetCamera(){return m_pCamera;}

protected:

private:
	CTerrain* m_pTerrain;
	CUserControlledCamera* m_pCamera;
};
