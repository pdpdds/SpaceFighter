#pragma once
#include "SFPlayerState.h"

class SFPlayerLive : public SFPlayerState
{
public:
	SFPlayerLive(SFPlayerPlane* pOwner);
	virtual ~SFPlayerLive(void);

	virtual BOOL OnEnter(){return FALSE;}
	virtual BOOL OnLeave(){return FALSE;}

	virtual BOOL OnRender(float fElapsedTime) override;
	virtual BOOL OnUpdate(float fElapsedTime) override;
	virtual BOOL ProcessInput(float fElapsedTime) override;
	virtual BOOL OnCheckInterAction(SFScene* pScene) override;

protected:

private:
	DWORD m_NextFireTime;
};
