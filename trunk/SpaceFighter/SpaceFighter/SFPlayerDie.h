#pragma once
#include "SFPlayerState.h"

class SFPlayerDie : public SFPlayerState
{
public:
	SFPlayerDie(SFPlayerPlane* pOwner);
	virtual ~SFPlayerDie(void);

	virtual BOOL OnEnter() override;
	virtual BOOL OnLeave() override;
	virtual BOOL OnUpdate(float fElapsedTime) override;

protected:

private:
	DWORD m_DieTime;
};
