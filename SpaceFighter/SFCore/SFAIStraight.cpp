#include "StdAfx.h"
#include "SFAIStraight.h"
#include "GameObject.h"
#include "SFPlane.h"

SFAIStraight::SFAIStraight(void)
{
}

SFAIStraight::~SFAIStraight(void)
{
}

BOOL SFAIStraight::ProcessAI(float fElapsedTime)
{
//Danger Code~~~
	SFPlane* pOwner = (SFPlane*)GetOwner();
	D3DXVECTOR3* pPos = pOwner->GetPosition();
	D3DXVECTOR3* pVel = pOwner->GetVelocity();
	pPos->z += pVel->z * fElapsedTime;

	return TRUE;
}