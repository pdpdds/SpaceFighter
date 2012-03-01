#pragma once
#include "SFPlane.h"

class SFBomb : public SFPlane
{
public:
	SFBomb(void);
	virtual ~SFBomb(void);

	virtual BOOL Init(GameObjectDesc& Desc) override;

	virtual BOOL Render(float fElapsedTime) override;
	virtual BOOL Update(float fElapsedTime) override;

protected:

private:
	float m_fTime;
	BOOL  m_bfEffect;
	float m_ScaleFactor;
	D3DXMATRIX m_matScaling; 
};
