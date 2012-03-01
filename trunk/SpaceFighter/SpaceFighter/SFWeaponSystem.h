#pragma once
#include <map>
#include "SFWeapon.h"

class GameObject;

class SFWeaponSystem
{
	typedef std::map<int, WeaponInfo> mapWeapon;

	friend class SFScene;
	friend class SFWeaponFireWork;
	friend class SFWeaponTwo;

public:
	SFWeaponSystem(GameObject* pObject);
	virtual ~SFWeaponSystem(void);

	BOOL AddWeapon(int type, WeaponInfo& Info); //무기를 추가한다.
	SFWeapon* CreateWeapon( int type, WeaponInfo& Info, SFWeapon::eMorphingMode ModeType = SFWeapon::Morphing_0);
	BOOL DeleteWeapon(int type); //무기를 지운다.
	BOOL CreateFireObject(float fElapsedTime); //미사일을 생성한다.
	BOOL IsPossible();

	GameObject* GetOwner(){return m_pOwner;}

	void ChangeWeapon();
	int GetWeapon(){return m_Weapon;}

protected:
	int m_CurrentFireObjectCount;

private:
	GameObject* m_pOwner;

	mapWeapon m_mapWeapon;

	int m_Weapon;
	int m_WeaponIndex;
	DWORD m_LastFiredTime;
};
