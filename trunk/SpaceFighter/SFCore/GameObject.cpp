#include "StdAfx.h"
#include "GameObject.h"
#include "OBB.h"

GameObject::GameObject(void)
: m_pTarget(NULL)
, m_bDisable(FALSE)
, m_pSFRes(NULL)
, m_pOBB(NULL)
, m_Scale(20)
, m_bInteract(TRUE)
, m_Health(1)
{
	static unsigned int ObjectID = 0;
	m_ObjectID = ObjectID;
	ObjectID++;

	D3DXMatrixIdentity( &m_Rotation );
}

GameObject::~GameObject(void)
{
	if(m_pOBB)
		delete m_pOBB;
}

BOOL GameObject::CheckIntersection( GameObject* pObject )
{
	if(NULL == pObject)
		return FALSE;

	OBB* pOBB = pObject->GetOBB();
	OBB* pMyOBB = GetOBB();

	if(pMyOBB->Intersect(*pOBB))
		return TRUE;

	return FALSE;
}
