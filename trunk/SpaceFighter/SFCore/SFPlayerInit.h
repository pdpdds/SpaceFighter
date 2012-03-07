#pragma once
#include "SFPlayerState.h"

class SFPlayerInit : public SFPlayerState
{
public:
	SFPlayerInit(SFPlayerPlane* pOwner);
	virtual ~SFPlayerInit(void);

	BOOL OnEnter();
	BOOL OnLeave();
};
