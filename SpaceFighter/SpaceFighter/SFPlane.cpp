#include "StdAfx.h"
#include "SFPlane.h"
#include "SFAIPattern.h"
#include "OBB.h"
#include "SFMessageDispatcher.h"

SFPlane::SFPlane(void)
: m_pCurPattern(NULL)
, m_WeaponSystem(this)
{
}

SFPlane::~SFPlane(void)
{
	listAIPattern::iterator iter = m_listAIPattern.begin();
	while(iter != m_listAIPattern.end())
	{
		SFAIPattern* pPattern = *iter;
		iter++;

		delete pPattern;
	}

	m_listAIPattern.clear();
}

BOOL SFPlane::AddPattern( SFAIPattern* pPattern )
{
	m_listAIPattern.push_back(pPattern);
	return TRUE;
}

BOOL SFPlane::Activate()
{
	listAIPattern::iterator iter = m_listAIPattern.begin();

	if(iter == m_listAIPattern.end())
		return FALSE;

	SFAIPattern* pPattern = *iter;

	m_pCurPattern = pPattern;

	m_listAIPattern.erase(iter);

	pPattern->OnEnter(this);

	return TRUE;
}

BOOL SFPlane::DeActivate()
{
	if(m_pCurPattern)
	{
		m_pCurPattern->OnLeave();
		delete m_pCurPattern;
		m_pCurPattern = NULL;
	}

	return TRUE;
}

BOOL SFPlane::Update( float fElapsedTime )
{
	if(m_pCurPattern)
	{
		if(FALSE == m_pCurPattern->ProcessAI(fElapsedTime))
		{
			DeActivate();
			Activate();
		}
	}

	OBB* pOBB = GetOBB();

	D3DXQUATERNION rot;

	D3DXQuaternionRotationMatrix(&rot, GetRotation());

	pOBB->m_rot += rot * fElapsedTime;
	
	D3DXQuaternionNormalize(&pOBB->m_rot, &pOBB->m_rot);
	pOBB->m_pos = *GetPosition();

	return TRUE;
}

BOOL SFPlane::ProcessInteraction( GameObject* pObject )
{
	if(this == pObject)
	{
		SFASSERT(1);
	}

	SFMessage msg;
	msg.MessageID = MSG_PLANE_DIE;
	SFMessageDispatcher::MessageDispatch(this, msg);
	delete this;

	return TRUE;
}

BOOL SFPlane::ProcessAttatcked( GameObject* pAttackObject )
{
	if(this == pAttackObject)
	{
		SFASSERT(1);
	}

	if(pAttackObject->GetObjectType() != OBJECT_WEAPON)
	{
		SFASSERT(1);
	} 

	SFWeapon* pWeapon = (SFWeapon*)pAttackObject;
	int Damage = pWeapon->GetWeaponInfo().Damage;

	if(GetHealth() - Damage <= 0)
	{
		SFMessage msg;
		msg.MessageID = MSG_PLANE_DIE;
		SFMessageDispatcher::MessageDispatch(this, msg);

		return TRUE;
	}

	return FALSE;
}