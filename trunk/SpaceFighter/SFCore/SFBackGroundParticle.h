#pragma once
#include "SFBackGround.h"

class CTerrain;
class CUserControlledCamera;

class SFBackGroundParticle : public SFBackGround
{
public:
	SFBackGroundParticle(void);
	virtual ~SFBackGroundParticle(void);

	virtual BOOL Init() override;

	virtual BOOL OnRender(float fElapsedTime) override;
	virtual BOOL OnResetDevice() override;
	virtual BOOL OnFrameMove( double fTime, float fElapsedTime) override;
	virtual BOOL OnLostDevice() override;
	virtual BOOL OnDestroyDevice() override;

	CUserControlledCamera* GetCamera(){return m_pCamera;}
	void RedrawFrame ( ) ;

protected:

private:
	CTerrain* m_pTerrain;
	CUserControlledCamera* m_pCamera;
	
};
