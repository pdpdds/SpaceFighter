#include "StdAfx.h"
#include "SFAIPattern.h"

SFAIPattern::SFAIPattern(void)
: m_pOwner(NULL)
, m_bActivate(FALSE)
, m_fTime(0.0f)
, m_bMisaileFire(FALSE)
{
	
}

SFAIPattern::~SFAIPattern(void)
{
}

void SFAIPattern::OnEnter( GameObject* pOwner )
{
	m_pOwner = pOwner;
	m_bActivate = TRUE;
}

void SFAIPattern::OnLeave()
{
	m_bActivate = FALSE;
}

BOOL SFAIPattern::ProcessAI(float fElapsedTime)
{
	return TRUE;
}