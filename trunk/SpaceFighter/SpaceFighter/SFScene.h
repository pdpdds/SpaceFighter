#pragma once
#include <list>
#include <map>
#include "SFMessageReceiver.h"

class SFBackGround;
class GameObject;
class SFWeapon;

class SFScene : public SFMessageReceiver
{
	typedef std::list<GameObject*> listGameObject;
	typedef std::map<int, GameObject*> mapGameObjectPool;
public:
	SFScene(void);
	virtual ~SFScene(void);

	virtual BOOL Init() = 0;

	virtual void AddBackGround(SFBackGround* pBackGround) {m_pBackGround = pBackGround;}
	virtual void AddGameObject(GameObject* pObject) {m_listGameObject.push_back(pObject);}
	virtual void AddGameObjectPool(int TimeLine, GameObject* pObject) {m_mapGameObjectPool.insert(mapGameObjectPool::value_type(TimeLine, pObject));}

	BOOL Render(float fElapsedTime);
	BOOL Update(float fTime, float fElapsedTime);
	virtual BOOL UpdateSpecificScene(float fElapsedTime) {return FALSE;};

	BOOL ResetDevice( IDirect3DDevice9* pd3dDevice );

	void SetHandlingObject(GameObject* pObject){m_pHandlingObject = pObject;}
	GameObject* GetHandlingObject(){return m_pHandlingObject;}

	BOOL BoundingSphereCollisionCheck(GameObject* pTarget, GameObject* pEnemy);
	BOOL SFScene::BoundingBoxCollisionCheck(GameObject* pTarget, GameObject* pEnemy);

	BOOL AddFireObject(SFWeapon* pWeapon);
	BOOL AddEnemyFireObject( SFWeapon* pWeapon );
	BOOL AddWaitingGameObjectWithTimeLine(float fElapsedTime);
	BOOL CheckPlayerEnemyIntersection();

	void SetWorldSize(D3DXVECTOR4& WorldSize){m_WorldSize = WorldSize;}
	D3DXVECTOR4& GetWorldSize(){return m_WorldSize;}
	BOOL IsOutofSceneArea(D3DXVECTOR3* pPos);

	virtual BOOL OnMessageEvent(GameObject* pSender, SFMessage& Msg) override;

protected:
	BOOL RenderBackGround(float fElapsedTime);
	BOOL RenderLayer(float fElapsedTime);
	BOOL RenderGameObject(float fElapsedTime);

private:
	SFBackGround* m_pBackGround;
	listGameObject m_listGameObject;
	listGameObject m_listFireObject;
	listGameObject m_listEnemyFireObject;
	mapGameObjectPool m_mapGameObjectPool;

	GameObject* m_pHandlingObject;

	float m_SceneStartTime;

	D3DXVECTOR4 m_WorldSize;
};
