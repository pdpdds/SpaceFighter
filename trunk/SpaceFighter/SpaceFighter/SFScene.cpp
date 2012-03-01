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
	GameObject* pGameObject = NULL;
	listGameObject::iterator iter = m_listGameObject.begin();

	for(;iter!= m_listGameObject.end(); iter++)
	{
		pGameObject = (*iter);
		pGameObject->Render(fElapsedTime);
	}

	iter = m_listFireObject.begin();
	for(;iter!= m_listFireObject.end(); iter++)
	{
		pGameObject = (*iter);
		pGameObject->Render(fElapsedTime);
	}

	iter = m_listEnemyFireObject.begin();
	for(;iter!= m_listEnemyFireObject.end(); iter++)
	{
		pGameObject = (*iter);
		pGameObject->Render(fElapsedTime);
	}

	if(m_pHandlingObject)
		m_pHandlingObject->Render(fElapsedTime);

	iter = m_listGameObject.begin();
	for(;iter!= m_listGameObject.end(); iter++)
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

	iter = m_listGameObject.begin();
	while(iter!= m_listGameObject.end())
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
	}

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
	GameObject* pGameObject = NULL;
	listGameObject::iterator iter = m_listGameObject.begin();

	for(;iter!= m_listGameObject.end(); iter++)
	{
		pGameObject = (*iter);
		pGameObject->Update(fElapsedTime);
	}

	iter = m_listGameObject.begin();
	while(iter!= m_listGameObject.end())
	{
		pGameObject = (*iter);
		if(pGameObject->GetDisable() == TRUE )//|| IsOutofSceneArea(pGameObject->GetPosition()))
		{
			iter = m_listGameObject.erase(iter);
			
			SFMessage msg;
			msg.MessageID = MSG_PLANE_DIE;
			SFMessageDispatcher::MessageDispatch(pGameObject, msg);
			delete pGameObject;
		}
		else
			iter++;
	}

//////////////////////////////////////////////////////////////////////
//Handling GameObject Update
//////////////////////////////////////////////////////////////////////
	GetHandlingObject()->Update(fElapsedTime);
	GetHandlingObject()->ProcessInput(fElapsedTime);

//////////////////////////////////////////////////////////////////////
//주인공의 미사일 객체들 업데이트
//////////////////////////////////////////////////////////////////////
	iter = m_listFireObject.begin();
	for(;iter!= m_listFireObject.end(); iter++)
	{
		pGameObject = (*iter);
		pGameObject->Update(fElapsedTime);
	}


	iter = m_listFireObject.begin();
	while(iter!= m_listFireObject.end())
	{
		SFWeapon* pWeapon = (SFWeapon*)(*iter);
		if(pWeapon->GetDisable() == TRUE || IsOutofSceneArea(pWeapon->GetPosition()))
		{
			iter = m_listFireObject.erase(iter);
			SFPlane* pObject = (SFPlane*)pWeapon->GetOwner();

			pObject->GetWeaponSystem()->m_CurrentFireObjectCount--;

			delete pWeapon;
		}
		else
			iter++;
	}

//////////////////////////////////////////////////////////////////////
//적 미사일 객체들 업데이트
//////////////////////////////////////////////////////////////////////
	iter = m_listEnemyFireObject.begin();
	for(;iter!= m_listEnemyFireObject.end(); iter++)
	{
		pGameObject = (*iter);
		pGameObject->Update(fElapsedTime);
	}


	iter = m_listEnemyFireObject.begin();
	while(iter!= m_listEnemyFireObject.end())
	{
		SFWeapon* pWeapon = (SFWeapon*)(*iter);
		if(pWeapon->GetDisable() == TRUE || IsOutofSceneArea(pWeapon->GetPosition()))
		{
			iter = m_listEnemyFireObject.erase(iter);
			SFPlane* pObject = (SFPlane*)pWeapon->GetOwner();
			
			if(NULL != pObject)
				pObject->GetWeaponSystem()->m_CurrentFireObjectCount--;

			delete pWeapon;
		}
		else
			iter++;
	}

//////////////////////////////////////////////////////////////////////
//주인공 비행기와 적기와의 충돌 체크
//////////////////////////////////////////////////////////////////////
	
	if(GetHandlingObject()->GetDisable() == FALSE)
	{
		CheckPlayerEnemyIntersection();
	}

//////////////////////////////////////////////////////////////////////
//주인공 무기와 오브젝트와의 충돌 체크
//////////////////////////////////////////////////////////////////////

	iter = m_listGameObject.begin();
	while(iter!= m_listGameObject.end())
	{
		BOOL bFlag = FALSE;
		listGameObject::iterator iter2 = m_listFireObject.begin();

		while(iter2 != m_listFireObject.end())
		{
			
			SFWeapon* pWeapon = (SFWeapon*)(*iter2);
			if(TRUE == pWeapon->CheckIntersection(*iter))
			{
				SFPlane* pPlane = (SFPlane*)(*iter);
				if(TRUE == pPlane->ProcessAttatcked(pWeapon))
				{
					delete pWeapon;

					iter2 = m_listFireObject.erase(iter2);
					iter = m_listGameObject.erase(iter);

					SFPlane* pPlane = (SFPlane*)GetHandlingObject();
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
	m_listFireObject.push_back((GameObject*)pWeapon);

	return TRUE;
}

BOOL SFScene::AddEnemyFireObject( SFWeapon* pWeapon )
{
	m_listEnemyFireObject.push_back((GameObject*)pWeapon);

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
	mapGameObjectPool::iterator iter1 = m_mapGameObjectPool.begin();
	mapGameObjectPool::iterator iter2 = m_mapGameObjectPool.begin();
	m_SceneStartTime += fElapsedTime;

	DWORD dwTime = m_SceneStartTime * 1000;

	while(iter1 != m_mapGameObjectPool.end())
	{
		iter2 = iter1;

		GameObject* pGameObject = iter1->second;

		if(dwTime < pGameObject->GetTimeLine())
			break;

		m_listGameObject.push_back(pGameObject);

		iter1++;
		m_mapGameObjectPool.erase(iter2);
	}
	
	return TRUE;
}

BOOL SFScene::CheckPlayerEnemyIntersection()
{
	listGameObject::iterator iter = m_listGameObject.begin();
	for(;iter!= m_listGameObject.end(); iter++)
	{
		GameObject* pGameObject = (*iter);
		if(TRUE == GetHandlingObject()->CheckIntersection(pGameObject))
		{
			
			if(TRUE == pGameObject->ProcessInteraction(GetHandlingObject()))
			{
				m_listGameObject.erase(iter);
			}

			GetHandlingObject()->SetDisable(TRUE);
			
			return TRUE;
		}
	}

	listGameObject::iterator iterFireObject = m_listEnemyFireObject.begin();
	for(;iterFireObject!= m_listEnemyFireObject.end(); iterFireObject++)
	{
		GameObject* pGameObject = (*iterFireObject);
		if(TRUE == GetHandlingObject()->CheckIntersection(pGameObject))
		{
			GetHandlingObject()->SetDisable(TRUE);
			delete pGameObject;
			m_listEnemyFireObject.erase(iterFireObject);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL SFScene::OnMessageEvent( GameObject* pSender, SFMessage& Msg )
{
	listGameObject::iterator iterFireObject = m_listEnemyFireObject.begin();
	for(;iterFireObject!= m_listEnemyFireObject.end(); iterFireObject++)
	{
		GameObject* pGameObject = (*iterFireObject);
		pGameObject->OnMessageEvent(pSender, Msg);
	}

	return TRUE;
}

BOOL SFScene::RenderLayer( float fElapsedTime )
{
	return TRUE;
}