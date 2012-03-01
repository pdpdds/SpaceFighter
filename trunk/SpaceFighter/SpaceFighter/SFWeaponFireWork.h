#pragma once
#include "SFWeapon.h"

class SFWeaponFireWork : public SFWeapon
{
public:
	SFWeaponFireWork(GameObject* pOwner, WeaponInfo& Info);
	virtual ~SFWeaponFireWork(void);

	virtual BOOL UpdateWeapon(float fElapsedTime) override;

protected:

private:
};