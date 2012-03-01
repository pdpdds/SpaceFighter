#pragma once
#include "SFPlane.h"

class SFBossBigBird : public SFPlane
{
public:
	SFBossBigBird(void);
	virtual ~SFBossBigBird(void);

	virtual BOOL Init(GameObjectDesc& Desc) override;

	virtual BOOL Render(float fElapsedTime) override;
	virtual BOOL Update(float fElapsedTime) override;

	virtual BOOL ProcessInteraction(GameObject* pObject) override;


protected:

private:
};
