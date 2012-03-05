#pragma once
#include "SFWeapon.h"

class SFWeaponFireWork : public SFWeapon
{
public:
	SFWeaponFireWork(SFPlane* pOwner, WeaponInfo& Info);
	virtual ~SFWeaponFireWork(void);

	virtual BOOL UpdateWeapon(float fElapsedTime) override;

protected:

private:
};