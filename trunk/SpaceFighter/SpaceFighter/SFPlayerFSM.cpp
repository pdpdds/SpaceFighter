#include "StdAfx.h"
#include "SFPlayerFSM.h"
#include "SFPlayerInit.h"
#include "SFPlayerLive.h"
#include "SFPlayerDie.h"

SFPlayerFSM::SFPlayerFSM(SFPlayerPlane* pOwner)
: SFPlayerState(pOwner)
, m_pCurrentState(NULL)
{
}

SFPlayerFSM::~SFPlayerFSM(void)
{
}


BOOL SFPlayerFSM::AddState( eSFPlayerState state )
{
	switch(state)
	{
	case SF_PLAYERSTATE_INIT:
		{
			SFPlayerState* pState = new SFPlayerInit(GetOwner());
			m_mapSFPlayerState.insert(mapSFPlayerState::value_type(state, pState));
			m_pCurrentState = pState;
		}
		break;
	case SF_PLAYERSTATE_LIVE:
		{
			SFPlayerState* pState = new SFPlayerLive(GetOwner());
			m_mapSFPlayerState.insert(mapSFPlayerState::value_type(state, pState));
		}
		break;
	case SF_PLAYERSTATE_DIE:
		{
			SFPlayerState* pState = new SFPlayerDie(GetOwner());
			m_mapSFPlayerState.insert(mapSFPlayerState::value_type(state, pState));
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

BOOL SFPlayerFSM::ChangeState( eSFPlayerState state )
{
	if(!m_pCurrentState)
		return FALSE;

	mapSFPlayerState::iterator iter = m_mapSFPlayerState.find(state);

	if(iter == m_mapSFPlayerState.end())
		return FALSE;

	if(iter->second == m_pCurrentState)
		return FALSE;

	m_pCurrentState->OnLeave();

	m_pCurrentState = iter->second;

	m_pCurrentState->OnEnter();

	return TRUE;
}

BOOL SFPlayerFSM::OnUpdate(float fElapsedTime )
{
	if(m_pCurrentState)
		return m_pCurrentState->OnUpdate(fElapsedTime);

	return FALSE;
}

BOOL SFPlayerFSM::OnRender(float fElapsedTime)
{
	if(m_pCurrentState)
		return m_pCurrentState->OnRender(fElapsedTime);

	return FALSE;
}

BOOL SFPlayerFSM::ProcessInput(float fElapsedTime)
{
	if(m_pCurrentState)
		return m_pCurrentState->ProcessInput(fElapsedTime);

	return FALSE;
}

BOOL SFPlayerFSM::OnCheckInterAction(SFScene* pScene)
{
	if(m_pCurrentState)
		return m_pCurrentState->OnCheckInterAction(pScene);

	return FALSE;
}
