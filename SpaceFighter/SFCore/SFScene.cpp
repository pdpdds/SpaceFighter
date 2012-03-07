#include "StdAfx.h"
#include "SFScene.h"
#include "SFBackGround.h"
#include "GameObject.h"
#include "SFSystem.h"
#include "DirectInput.h"
#include "SFResMesh.h"
#include "SFWeapon.h"
#include "SFPlane.h"
#include "SFMessageDispatcher.h"
#include "SFPlayerFSM.h"
#include "SFPlayerPlane.h"

SFScene::SFScene(void)
: m_pBackGround(NULL)
, m_pHandlingObject(NULL)
{
	m_SceneStartTime = 0;
}

SFScene::~SFScene(void)
{
}

BOOL SFScene::Render( float fElapsedTime )
{
	RenderBackGround(fElapsedTime);
	RenderLayer(fElapsedTime);
	RenderGameObject(fElapsedTime);

	return TRUE;
}

BOOL SFScene::RenderBackGround( float fElapsedTime )
{
	if(m_pBackGround)
		m_pBackGround->OnRender(fElapsedTime);

	return TRUE;
}

BOOL SFScene::RenderGameObject( float fElapsedTime )
{
	SFPlane* pPlane = NULL;
	listPlane::iterator iter = m_listPlane.begin();

	for(;iter!= m_listPlane.end(); iter++)
	{
		pPlane = (*iter);
		pPlane->Render(fElapsedTime);
	}

	SFWeapon* pWeapon = NULL;

	listWeapon::iterator iter2 = m_listFireObject.begin();
	for(;iter2!= m_listFireObject.end(); iter2++)
	{
		pWeapon = (*iter2);
		pWeapon->Render(fElapsedTime);
	}

	iter2 = m_listEnemyFireObject.begin();
	for(;iter2!= m_listEnemyFireObject.end(); iter2++)
	{
		pWeapon = (*iter2);
		pWeapon->Render(fElapsedTime);
	}

	if(m_pHandlingObject)
		m_pHandlingObject->Render(fElapsedTime);

	/*iter = m_listEnemyObject.begin();
	for(;iter!= m_listEnemyObject.end(); iter++)
	{
		pGameObject = (*iter);
		//BoundingSphereCollisionCheck(GetHandlingObject(), pGameObject);
		if(TRUE == GetHandlingObject()->CheckIntersection(pGameObject))
		{
			TCHAR strPass[256];
			_stprintf_s(strPass, L"InterSect");

			RECT rc;
			SetRect( &rc, 0, 20, 300, 0 );
			SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
			pSystem->GetDebugFont()->Print(strPass, rc,  D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
		}
	}

	iter = m_listEnemyObject.begin();
	while(iter!= m_listEnemyObject.end())
	{
		listGameObject::iterator iter2 = m_listFireObject.begin();

		while(iter2 != m_listFireObject.end())
		{


			SFWeapon* pWeapon = (SFWeapon*)(*iter2);
			if(TRUE == pWeapon->CheckIntersection(*iter))
			{
				TCHAR strPass[256];
				_stprintf_s(strPass, L"Misaile Attack");

				RECT rc;
				SetRect( &rc, 0, 40, 300, 0 );
				SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
				pSystem->GetDebugFont()->Print(strPass, rc,  D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));

			}
			iter2++;
		}

		iter++;
	}*/

	return TRUE;
}

BOOL SFScene::Update(float fTime, float fElapsedTime )
{
//////////////////////////////////////////////////////////////////////
//Input State Update
//////////////////////////////////////////////////////////////////////
	SFSystem::GetInstance()->GetInputSystem()->poll();

//////////////////////////////////////////////////////////////////////
//BackGround Update
//////////////////////////////////////////////////////////////////////
	m_pBackGround->OnFrameMove(fTime, fElapsedTime);

//////////////////////////////////////////////////////////////////////
//GameObject Update
//////////////////////////////////////////////////////////////////////
	UpdateEnemyObject(fElapsedTime);

//////////////////////////////////////////////////////////////////////
//Handling GameObject Update
//////////////////////////////////////////////////////////////////////

	GetHandlingObject()->Update(fElapsedTime);
	GetHandlingObject()->ProcessInput(fElapsedTime);

//////////////////////////////////////////////////////////////////////
//주인공의 미사일 객체들 업데이트
//////////////////////////////////////////////////////////////////////
	UpdateFireObject(fElapsedTime);

//////////////////////////////////////////////////////////////////////
//적 미사일 객체들 업데이트
//////////////////////////////////////////////////////////////////////
	UpdateEnemyFireObject(fElapsedTime);

//////////////////////////////////////////////////////////////////////
//주인공 비행기와 적기와의 충돌 체크
//////////////////////////////////////////////////////////////////////
	OnCheckInterAction();

//////////////////////////////////////////////////////////////////////
//주인공 무기와 오브젝트와의 충돌 체크
//////////////////////////////////////////////////////////////////////
	CheckPlayerWeaponEnemyIntersection(fElapsedTime);

//////////////////////////////////////////////////////////////////////
//해당 시간에 등장할 적기 등록
//////////////////////////////////////////////////////////////////////	
	AddWaitingGameObjectWithTimeLine(fElapsedTime);

	return UpdateSpecificScene(fElapsedTime);
}

BOOL SFScene::ResetDevice( IDirect3DDevice9* pd3dDevice )
{
	if(m_pBackGround)
	m_pBackGround->OnResetDevice();

	return TRUE;
}

BOOL SFScene::BoundingSphereCollisionCheck(GameObject* pTarget, GameObject* pEnemy)
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	D3DXVECTOR3* pPos1 = pTarget->GetPosition();
	D3DXVECTOR3* pPos2 = pEnemy->GetPosition();

	D3DXVECTOR3 vecDistance = *pPos1 - *pPos2;

	float fDistance = D3DXVec3Length(&vecDistance);

	SFResMesh* pRes = (SFResMesh*)pSystem->GetResMgr()->FindResource(pTarget->GetObjectType());
	SFResMesh* pRes2 = (SFResMesh*)pSystem->GetResMgr()->FindResource(pEnemy->GetObjectType());
	
	float fDistance2 = pRes->GetRadius() +  pRes2->GetRadius();

	if(fDistance < fDistance2)
		return TRUE;

	return FALSE;
}

BOOL SFScene::BoundingBoxCollisionCheck(GameObject* pTarget, GameObject* pEnemy)
{
	SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
	D3DXVECTOR3* pPos1 = pTarget->GetPosition();
	D3DXVECTOR3* pPos2 = pEnemy->GetPosition();

	SFResMesh* pRes = (SFResMesh*)pSystem->GetResMgr()->FindResource(pTarget->GetObjectType());
	SFResMesh* pRes2 = (SFResMesh*)pSystem->GetResMgr()->FindResource(pEnemy->GetObjectType());

	return FALSE;
}

BOOL SFScene::AddFireObject( SFWeapon* pWeapon )
{
	m_listFireObject.push_back(pWeapon);

	return TRUE;
}

BOOL SFScene::AddEnemyFireObject( SFWeapon* pWeapon )
{
	m_listEnemyFireObject.push_back(pWeapon);

	return TRUE;
}

BOOL SFScene::IsOutofSceneArea( D3DXVECTOR3* pPos )
{
	if (pPos->x < m_WorldSize.x || 
		pPos->x > m_WorldSize.y ||
		pPos->z < m_WorldSize.z || 
		pPos->z > m_WorldSize.w)
		return TRUE;

	return FALSE;
}

BOOL SFScene::AddWaitingGameObjectWithTimeLine(float fElapsedTime)
{
	mapPlanePool::iterator iter1 = m_mapPlanePool.begin();
	mapPlanePool::iterator iter2;
	m_SceneStartTime += fElapsedTime;

	DWORD dwTime = m_SceneStartTime * 1000;

	while(iter1 != m_mapPlanePool.end())
	{
		iter2 = iter1;

		SFPlane* pPlane = iter1->second;

		if(dwTime < pPlane->GetTimeLine())
			break;

		m_listPlane.push_back(pPlane);

		iter1++;
		m_mapPlanePool.erase(iter2);
	}
	
	return TRUE;
}

BOOL SFScene::OnCheckInterAction()
{
	return GetHandlingObject()->GetFSM()->OnCheckInterAction(this);
}

BOOL SFScene::OnMessageEvent( GameObject* pSender, SFMessage& Msg )
{
	if (Msg.ObjectType == OBJECT_ALL || Msg.ObjectType == OBJECT_WEAPON)
	{
		listWeapon::iterator iterFireObject = m_listEnemyFireObject.begin();
		for(;iterFireObject!= m_listEnemyFireObject.end(); iterFireObject++)
		{
			SFWeapon* pWeapon = (*iterFireObject);
			pWeapon->OnMessageEvent(pSender, Msg);
		}
	}

	if (Msg.ObjectType == OBJECT_ALL)
	{
		listPlane::iterator Plane = m_listPlane.begin();
		for(;Plane!= m_listPlane.end(); Plane++)
		{
			SFPlane* pPlane = (*Plane);
			pPlane->OnMessageEvent(pSender, Msg);
		}
	}

	return TRUE;
}

BOOL SFScene::RenderLayer( float fElapsedTime )
{
	return TRUE;
}

BOOL SFScene::UpdateEnemyObject( float fElapsedTime )
{
	SFPlane* pPlane = NULL;
	listPlane::iterator iter = m_listPlane.begin();

	for(;iter!= m_listPlane.end(); iter++)
	{
		pPlane = (*iter);
		pPlane->Update(fElapsedTime);
	}

	iter = m_listPlane.begin();
	while(iter!= m_listPlane.end())
	{
		pPlane = (*iter);
		if(pPlane->GetDisable() == TRUE )//|| IsOutofSceneArea(pGameObject->GetPosition()))
		{
			iter = m_listPlane.erase(iter);

			SFMessage msg;
			msg.MessageID = MSG_PLANE_DIE;
			msg.ObjectType = OBJECT_WEAPON;
			SFMessageDispatcher::MessageDispatch(pPlane, msg);
			delete pPlane;
		}
		else
			iter++;
	}

	return TRUE;
}

BOOL SFScene::UpdateFireObject( float fElapsedTime )
{
	SFWeapon* pWeapon = NULL;

	listWeapon::iterator iter = m_listFireObject.begin();
	for(;iter!= m_listFireObject.end(); iter++)
	{
		pWeapon = (*iter);
		pWeapon->Update(fElapsedTime);
	}


	iter = m_listFireObject.begin();
	while(iter!= m_listFireObject.end())
	{
		SFWeapon* pWeapon = (*iter);
		if(pWeapon->GetDisable() == TRUE || IsOutofSceneArea(pWeapon->GetPosition()))
		{
			iter = m_listFireObject.erase(iter);
			SFPlane* pObject = pWeapon->GetOwner();

			pObject->GetWeaponSystem()->m_CurrentFireObjectCount--;

			delete pWeapon;
		}
		else
			iter++;
	}

	return TRUE;
}



BOOL SFScene::UpdateEnemyFireObject( float fElapsedTime )
{
	SFWeapon* pWeapon = NULL;

	listWeapon::iterator iter = m_listEnemyFireObject.begin();

	for(;iter!= m_listEnemyFireObject.end(); iter++)
	{
		pWeapon = (*iter);
		pWeapon->Update(fElapsedTime);
	}

	iter = m_listEnemyFireObject.begin();
	while(iter!= m_listEnemyFireObject.end())
	{
		pWeapon = (*iter);
		if(pWeapon->GetDisable() == TRUE || IsOutofSceneArea(pWeapon->GetPosition()))
		{
			iter = m_listEnemyFireObject.erase(iter);
			SFPlane* pPlane = pWeapon->GetOwner();

			if(NULL != pPlane)
				pPlane->GetWeaponSystem()->m_CurrentFireObjectCount--;

			delete pWeapon;
		}
		else
			iter++;
	}

	return TRUE;
}

BOOL SFScene::CheckPlayerWeaponEnemyIntersection( float fElapsedTime )
{
	listPlane::iterator iter = m_listPlane.begin();
	while(iter!= m_listPlane.end())
	{
		BOOL bFlag = FALSE;

		listWeapon::iterator iter2 = m_listFireObject.begin();

		while(iter2 != m_listFireObject.end())
		{

			SFWeapon* pWeapon = *iter2;
			if(TRUE == pWeapon->CheckIntersection(*iter) && (*iter)->GetInteract() == TRUE)
			{
				SFPlane* pPlane = (SFPlane*)(*iter);
				if(TRUE == pPlane->ProcessAttatcked(pWeapon))
				{
					delete pWeapon;

					iter2 = m_listFireObject.erase(iter2);
					iter = m_listPlane.erase(iter);

					SFPlane* pPlane = GetHandlingObject();
					pPlane->GetWeaponSystem()->m_CurrentFireObjectCount--;

					bFlag = TRUE;

					break;
				}



			}
			else
			{
				iter2++;
			}
		}

		if(bFlag == FALSE)
			iter++;
	}
	return TRUE;
}
