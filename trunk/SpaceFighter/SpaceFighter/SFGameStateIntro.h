#pragma once
#include "SFGameState.h"

class SFGameStateIntro : public SFGameState
{
public:
	SFGameStateIntro(void);
	virtual ~SFGameStateIntro(void);

	virtual BOOL OnRender(float fElapsedTime) override;
	virtual BOOL OnUpdate(float fTime, float fElapsedTime) override;
};
