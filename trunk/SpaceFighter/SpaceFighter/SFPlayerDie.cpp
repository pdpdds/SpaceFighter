#include "StdAfx.h"
#include "SFPlayerDie.h"
#include "SFPlayerPlane.h"
#include "SFPlayerFSM.h"

SFPlayerDie::SFPlayerDie(SFPlayerPlane* pOwner)
: SFPlayerState(pOwner)
{
}

SFPlayerDie::~SFPlayerDie(void)
{
}

BOOL SFPlayerDie::OnEnter()
{
	m_DieTime = GetTickCount();
	return TRUE;
}

BOOL SFPlayerDie::OnLeave()
{
	m_DieTime = 0;
	return TRUE;
}

BOOL SFPlayerDie::OnUpdate(float fElapsedTime)
{
	if ((GetTickCount() - m_DieTime) > 3000)
	{
		GetOwner()->GetFSM()->ChangeState(SF_PLAYERSTATE_LIVE);
	}

	return TRUE;
}
