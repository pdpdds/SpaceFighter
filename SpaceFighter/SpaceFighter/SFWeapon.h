#pragma once
#include "GameObject.h"

class SFWeapon : public GameObject
{
	friend class SFWeaponSystem;

public:
	enum eMorphingMode
	{
		Morphing_0 = 0,
		Morphing_1,
		Morphing_2,
	};

	SFWeapon( GameObject* pOwner, WeaponInfo& Info );
	virtual ~SFWeapon(void);

	virtual BOOL Update(float fElapsedTime) override;
	virtual BOOL Render(float fElapsedTime) override;
	GameObject* GetOwner(){return m_pOwner;}
	void SetOwner(GameObject* pObject){m_pOwner = pObject;}

	WeaponInfo& GetWeaponInfo(){return m_WeaponInfo;}

	virtual BOOL UpdateWeapon(float fElapsedTime) = 0;

	void SetMode(eMorphingMode Mode){m_eMode = Mode;}
	eMorphingMode GetMorphingMode(){return m_eMode;}

	virtual BOOL OnMessageEvent(GameObject* pSender, SFMessage& Msg) override;

protected:

private:
	GameObject* m_pOwner; //해당 발사체의 소유주

	WeaponInfo m_WeaponInfo;
	eMorphingMode m_eMode;

	CTimer Timer;
};
