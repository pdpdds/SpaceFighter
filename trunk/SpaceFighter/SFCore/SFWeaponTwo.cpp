#include "StdAfx.h"
#include "SFWeaponTwo.h"
#include "SFSystem.h"
#include "sdksound.h"
#include "SFScene.h"
#include "SFPlane.h"
#include "SFWeaponSystem.h"

extern CSound* g_pSoundFire;

SFWeaponTwo::SFWeaponTwo( SFPlane* pOwner, WeaponInfo& Info )
: SFWeapon(pOwner, Info)
{

}

SFWeaponTwo::~SFWeaponTwo( void )
{

}

BOOL SFWeaponTwo::UpdateWeapon( float fElapsedTime )
{
	if(GetDisable() == TRUE)
		return TRUE;

	D3DXVECTOR3* pPos = GetPosition();
	D3DXVECTOR3* pVel = GetVelocity();

	*pPos += fElapsedTime * (*GetVelocity());

	if(SFWeapon::Morphing_0 == GetMorphingMode())
	{
		if(timeGetTime() - GetWeaponInfo().FireTime > 10)
		{
			SetDisable(TRUE);

			D3DXVECTOR3 newVec = *GetPosition();
			newVec.x += -5.0f;

			WeaponInfo Info = GetWeaponInfo();
			Info.vecStartPos = newVec;

			SFPlane* pObject = (SFPlane*)GetOwner();
			SFWeaponSystem* pWeaponSystem = pObject->GetWeaponSystem();

			pWeaponSystem->CreateWeapon(WEAPON_TWO, Info, SFWeapon::Morphing_1);

			newVec.x += 10.0f;
			Info.vecStartPos = newVec;
			pWeaponSystem->CreateWeapon(WEAPON_TWO, Info, SFWeapon::Morphing_1);
		}
	}

	return TRUE;
}