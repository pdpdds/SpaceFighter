#pragma once
#include "SFPlane.h"

class SFNormalPlane : public SFPlane
{
public:
	SFNormalPlane();
	virtual ~SFNormalPlane(void);

	virtual BOOL Init(GameObjectDesc& Desc) override;

	virtual BOOL Render(float fElapsedTime) override;
	virtual BOOL Update(float fElapsedTime) override;

};
