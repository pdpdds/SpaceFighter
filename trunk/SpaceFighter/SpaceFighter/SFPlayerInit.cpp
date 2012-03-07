#include "StdAfx.h"
#include "SFPlayerInit.h"

SFPlayerInit::SFPlayerInit(SFPlayerPlane* pOwner)
: SFPlayerState(pOwner)
{
}

SFPlayerInit::~SFPlayerInit(void)
{
}

BOOL SFPlayerInit::OnEnter()
{
	return TRUE;
}

BOOL SFPlayerInit::OnLeave()
{
	return TRUE;
}
