#pragma once
#include "SFGameState.h"

class SFGameStatePlay : public SFGameState
{
public:
	SFGameStatePlay(void);
	virtual ~SFGameStatePlay(void);

	virtual BOOL OnEnter() override;

	virtual BOOL OnRender(float fElapsedTime) override;
	virtual BOOL OnUpdate(float fTime, float fElapsedTime) override;
	
};
