#include "StdAfx.h"
#include "SFWeaponFireWork.h"
#include "SFSystem.h"
#include "sdksound.h"
#include "SFScene.h"
#include "SFPlane.h"
#include "SFWeaponSystem.h"
#include "SFResAni.h"
#include "OBB.h"

extern CSound* g_pSoundFire;

SFWeaponFireWork::SFWeaponFireWork( GameObject* pOwner, WeaponInfo& Info )
: SFWeapon(pOwner, Info)
{
	
}

SFWeaponFireWork::~SFWeaponFireWork(void)
{
}

BOOL SFWeaponFireWork::UpdateWeapon( float fElapsedTime )
{
	if(GetDisable() == TRUE)
		return TRUE;

	D3DXVECTOR3* pPos = GetPosition();
	D3DXVECTOR3* pVel = GetVelocity();

	*pPos += fElapsedTime * (*GetVelocity());

	if(SFWeapon::Morphing_0 == GetMorphingMode())
	{
		if(timeGetTime() - GetWeaponInfo().FireTime > 700)
		{
			SetDisable(TRUE);

			GetWeaponInfo().FireTime = timeGetTime();

			float fAngle = (2.0f *PI) / 8.0f;
			D3DXMATRIX matRotation;
			D3DXVECTOR3 vecDir(1.0f, 0.0f, 0.5f);
			D3DXVec3Normalize(&vecDir, &vecDir);

			SFPlane* pObject = (SFPlane*)GetOwner();
			SFWeaponSystem* pWeaponSystem = pObject->GetWeaponSystem();

			for(int i = 0; i < 8; i++ )
			{
				D3DXMatrixRotationY(&matRotation, fAngle);
				D3DXVec3TransformCoord(&vecDir, &vecDir, &matRotation);	
				D3DXVec3Normalize(&vecDir, &vecDir);

				WeaponInfo Info = GetWeaponInfo();
				Info.vecVelocity = vecDir * D3DXVec3Length(GetVelocity());
				Info.vecStartPos = *GetPosition();

				pWeaponSystem->CreateWeapon(WEAPON_FIREWORK, Info, SFWeapon::Morphing_1);
				
				
			}

			g_pSoundFire->Play( 0, 0L);
		}
	}

	return TRUE;
}