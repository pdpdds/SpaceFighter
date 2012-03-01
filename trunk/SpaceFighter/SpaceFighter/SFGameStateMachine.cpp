#include "StdAfx.h"
#include "SFGameStateMachine.h"
#include "SFGameStateInit.h"
#include "SFGameStateIntro.h"
#include "SFGameStateTitle.h"
#include "SFGameStateOption.h"
#include "SFGameStatePlay.h"
#include "SFGameStateEnding.h"
#include "SFGameStatePrologue.h"

SFGameStateMachine::SFGameStateMachine(void)
{
	m_pCurrentState = NULL;
}

SFGameStateMachine::~SFGameStateMachine(void)
{
}

BOOL SFGameStateMachine::AddState( eSFGameState state )
{
	switch(state)
	{
	case SF_GAMESTATE_INIT:
		{
			SFGameState* pState = new SFGameStateInit();
			m_mapSFGameState.insert(mapSFGameState::value_type(state, pState));
			m_pCurrentState = pState;
		}
		break;
	case SF_GAMESTATE_INTRO:
		{
			SFGameState* pState = new SFGameStateIntro();
			m_mapSFGameState.insert(mapSFGameState::value_type(state, pState));
		}
		break;
	case SF_GAMESTATE_TITLE:
		{
			SFGameState* pState = new SFGameStateTitle();
			m_mapSFGameState.insert(mapSFGameState::value_type(state, pState));
		}
		break;
	case SF_GAMESTATE_PROLOGUE:
		{
			SFGameState* pState = new SFGameStatePrologue();
			m_mapSFGameState.insert(mapSFGameState::value_type(state, pState));
		}
		break;
	case SF_GAMESTATE_PLAY:
		{
			SFGameState* pState = new SFGameStatePlay();
			m_mapSFGameState.insert(mapSFGameState::value_type(state, pState));
		}
		break;
	case SF_GAMESTATE_ENDING:
		{
			SFGameState* pState = new SFGameStateEnding();
			m_mapSFGameState.insert(mapSFGameState::value_type(state, pState));
		}
		break;
	case SF_GAMESTATE_OPTION:
		{
			SFGameState* pState = new SFGameStateOption();
			m_mapSFGameState.insert(mapSFGameState::value_type(state, pState));
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

BOOL SFGameStateMachine::ChangeState( eSFGameState state )
{
	if(!m_pCurrentState)
		return FALSE;

	mapSFGameState::iterator iter = m_mapSFGameState.find(state);

	if(iter == m_mapSFGameState.end())
		return FALSE;

	if(iter->second == m_pCurrentState)
		return FALSE;

	m_pCurrentState->OnLeave();

	m_pCurrentState = iter->second;

	m_pCurrentState->OnEnter();

	return TRUE;
}

BOOL SFGameStateMachine::OnRender(float fElapsedTime)
{
	if(m_pCurrentState)
		return m_pCurrentState->OnRender(fElapsedTime);

	return FALSE;
}

BOOL SFGameStateMachine::OnUpdate(float fTime, float fElapsedTime )
{
	if(m_pCurrentState)
		return m_pCurrentState->OnUpdate(fTime, fElapsedTime);

	return FALSE;
}