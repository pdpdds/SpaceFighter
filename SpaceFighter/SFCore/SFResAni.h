#pragma once
#include "SFRes.h"

class CSprite;

class SFResAni : public SFRes
{
public:
	SFResAni(CSprite* pSprite);
	virtual ~SFResAni(void);

	virtual BOOL Render(float fElapsedTime) override;
	virtual BOOL Render(float fElapsedTime, GameObject* pObject) override;
	virtual BOOL Render(float fElapsedTime, GameObject* pObject, CTimer& Timer) override;
	virtual BOOL Init() override;


protected:

private:
	CSprite* m_pSprite;
};
