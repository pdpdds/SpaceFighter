#include "StdAfx.h"
#include "SFWeaponSystem.h"
#include "SFWeapon.h"
#include "sdksound.h"
#include "SFSystem.h"
#include "SFScene.h"
#include "SFWeaponDefault.h"
#include "SFWeaponFireWork.h"
#include "SFWeaponTwo.h"

extern CSound* g_pSoundFire;

SFWeaponSystem::SFWeaponSystem( GameObject* pObject )
: m_pOwner(pObject)
, m_CurrentFireObjectCount(0)
, m_LastFiredTime(0)
, m_Weapon(WEAPON_DEFAULT)
, m_WeaponIndex(0)
{

}
SFWeaponSystem::~SFWeaponSystem(void)
{
}

BOOL SFWeaponSystem::AddWeapon( int type, WeaponInfo& Info)
{
	m_mapWeapon.insert(mapWeapon::value_type(type, Info));
	return TRUE;
}

BOOL SFWeaponSystem::CreateFireObject( float fElapsedTime )
{
	D3DXVECTOR3* pPos = GetOwner()->GetPosition();

	mapWeapon::iterator iter = m_mapWeapon.find(GetWeapon());

	WeaponInfo Info = iter->second;

	Info.vecStartPos = *pPos;

	if(FALSE == IsPossible())
		return FALSE;

	SFWeapon* pWeapon = CreateWeapon(GetWeapon(), Info, SFWeapon::Morphing_0);

	if(NULL == pWeapon)
		return FALSE;

	g_pSoundFire->Play( 0, 0L);
	
	return TRUE;
}

BOOL SFWeaponSystem::IsPossible()
{
	if(WEAPON_DEFAULT == GetWeapon())
	{
		if(m_CurrentFireObjectCount < 3)
			return TRUE;
	}
	else if(WEAPON_TWO == GetWeapon())
	{
		if(m_CurrentFireObjectCount < 3)
			return TRUE;
	}
	else if(WEAPON_FIREWORK == GetWeapon())
	{
		if(m_CurrentFireObjectCount < 1)
			return TRUE;
	}
	return FALSE;
}

BOOL SFWeaponSystem::DeleteWeapon( int type )
{
	return TRUE;
}

SFWeapon* SFWeaponSystem::CreateWeapon( int type, WeaponInfo& Info, SFWeapon::eMorphingMode ModeType )
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	SFWeapon* pWeapon = NULL;

	if(type == WEAPON_DEFAULT)
	{
		pWeapon = new SFWeaponDefault(GetOwner(), Info);
	}
	else if(type == WEAPON_TWO)
	{
		pWeapon = new SFWeaponTwo(GetOwner(), Info);
	}
	else if(type == WEAPON_FIREWORK)
	{
		pWeapon = new SFWeaponFireWork(GetOwner(), Info);
	}

	if(pWeapon != NULL)
	{
		pWeapon->SetObjectType(OBJECT_WEAPON);

		pWeapon->SetMode(ModeType);
		if(GetOwner()->GetObjectType() == OBJECT_PLAYER)
		{
			pSystem->GetCurrentScene()->AddFireObject(pWeapon);
		}
		else
		{
			pSystem->GetCurrentScene()->AddEnemyFireObject(pWeapon);
		}
		
		m_CurrentFireObjectCount++;
	}

	return pWeapon;
}

void SFWeaponSystem::ChangeWeapon()
{
	int iSize = m_mapWeapon.size();

	m_WeaponIndex++;

	if(m_WeaponIndex >= iSize)
		m_WeaponIndex = 0;

	int WeaponIndex = m_WeaponIndex;
	mapWeapon::iterator iter = m_mapWeapon.begin();

	while(WeaponIndex)
	{
		iter++;
		WeaponIndex--;
	}

	m_Weapon = iter->first;
}