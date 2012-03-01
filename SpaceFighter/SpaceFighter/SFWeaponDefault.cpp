#include "StdAfx.h"
#include "SFWeaponDefault.h"
#include "SFSystem.h"
#include "SFResAni.h"
#include "OBB.h"

SFWeaponDefault::SFWeaponDefault( GameObject* pOwner, WeaponInfo& Info )
: SFWeapon(pOwner, Info)
{
	
}

SFWeaponDefault::~SFWeaponDefault(void)
{
}

BOOL SFWeaponDefault::UpdateWeapon( float fElapsedTime )
{
	D3DXVECTOR3* pPos = GetPosition();

	*pPos += fElapsedTime * (*GetVelocity());
	
	return TRUE;
}