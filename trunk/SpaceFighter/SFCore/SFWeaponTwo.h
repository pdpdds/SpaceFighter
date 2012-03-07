#pragma once
#include "SFWeapon.h"

class SFWeaponTwo :  public SFWeapon
{
	enum eTwoMode
	{
		TWO_MODE0 = 0,
		TWO_MODE1,
	};

public:
	SFWeaponTwo(SFPlane* pOwner, WeaponInfo& Info);
	virtual ~SFWeaponTwo(void);

	virtual BOOL UpdateWeapon(float fElapsedTime) override;

	

protected:

private:
};
