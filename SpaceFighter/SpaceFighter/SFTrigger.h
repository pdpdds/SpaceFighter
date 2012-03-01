#pragma once
#include "SFPlane.h"

class SFTrigger : public SFPlane
{
public:
	SFTrigger(void);
	virtual ~SFTrigger(void);

	virtual BOOL Init(GameObjectDesc& Desc) override;

	virtual BOOL Render(float fElapsedTime) override;
	virtual BOOL Update(float fElapsedTime) override;

	virtual BOOL ProcessInteraction(GameObject* pObject) override;

protected:

private:
};
