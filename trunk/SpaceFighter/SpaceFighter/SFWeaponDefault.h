#pragma once
#include "SFWeapon.h"

class SFWeaponDefault :  public SFWeapon
{
public:
	SFWeaponDefault(GameObject* pOwner, WeaponInfo& Info);
	virtual ~SFWeaponDefault(void);

	virtual BOOL UpdateWeapon(float fElapsedTime) override;

protected:

private:
};
