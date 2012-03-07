#include "StdAfx.h"
#include "SFPlayerDie.h"
#include "SFPlayerPlane.h"
#include "SFPlayerFSM.h"
#include "SFMessageDispatcher.h"

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

	SFMessage msg;
	msg.MessageID = MSG_PLAYER_DIE;
	msg.ObjectType = OBJECT_ALL;
	
	SFMessageDispatcher::MessageDispatch(GetOwner(), msg);

	return TRUE;
}

BOOL SFPlayerDie::OnLeave()
{
	m_DieTime = 0;

	GetOwner()->SetPosition(D3DXVECTOR3(0,0,-50));

	SFMessage msg;
	msg.MessageID = MSG_PLAYER_LIVE;
	msg.ObjectType = OBJECT_ALL;
	
	SFMessageDispatcher::MessageDispatch(GetOwner(), msg);
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
