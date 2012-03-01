#pragma once
#include "GameObject.h"
#include <list>
#include "SFWeaponSystem.h"

class SFAIPattern;

class SFPlane : public GameObject
{
	typedef std::list<SFAIPattern*> listAIPattern;

public:
	SFPlane(void);
	virtual ~SFPlane(void);

	virtual BOOL Init(GameObjectDesc& Desc){return FALSE;}
	BOOL Activate();
	BOOL DeActivate();
	BOOL Update(float fElapsedTimer);
	virtual BOOL ProcessInteraction(GameObject* pObject) override;
	virtual BOOL ProcessAttatcked(GameObject* pAttackObject) override;

	SFWeaponSystem* GetWeaponSystem(){return &m_WeaponSystem;}

protected:
	BOOL AddPattern(SFAIPattern* pPattern);

private:
	listAIPattern	m_listAIPattern;
	SFAIPattern* m_pCurPattern;
	SFWeaponSystem m_WeaponSystem;
};