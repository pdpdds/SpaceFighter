#pragma once
#include <list>
#include <map>
#include "SFMessageReceiver.h"

class SFBackGround;
class GameObject;
class SFWeapon;
class SFPlane;

class SFScene : public SFMessageReceiver
{
	typedef std::list<SFPlane*> listPlane;
	typedef std::list<SFWeapon*> listWeaphon;
	typedef std::map<int, SFPlane*> mapPlanePool;
public:
	SFScene(void);
	virtual ~SFScene(void);

	virtual BOOL Init() = 0;

	virtual void AddBackGround(SFBackGround* pBackGround) {m_pBackGround = pBackGround;}
	virtual void AddPlane(SFPlane* pPlane) {m_listPlane.push_back(pPlane);}
	virtual void AddPlanePool(int TimeLine, SFPlane* pPlane) {m_mapPlanePool.insert(mapPlanePool::value_type(TimeLine, pPlane));}

	BOOL Render(float fElapsedTime);
	BOOL Update(float fTime, float fElapsedTime);
	virtual BOOL UpdateSpecificScene(float fElapsedTime) {return FALSE;};

	BOOL ResetDevice( IDirect3DDevice9* pd3dDevice );

	void SetHandlingObject(SFPlane* pPlane){m_pHandlingObject = pPlane;}
	SFPlane* GetHandlingObject(){return m_pHandlingObject;}

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
	listPlane m_listPlane;
	listWeaphon m_listFireObject;
	listWeaphon m_listEnemyFireObject;
	mapPlanePool m_mapPlanePool;

	SFPlane* m_pHandlingObject;

	float m_SceneStartTime;

	D3DXVECTOR4 m_WorldSize;

protected:
	BOOL UpdateEnemyObject(float fElapsedTime);
	BOOL UpdateFireObject(float fElapsedTime);
	BOOL UpdateEnemyFireObject( float fElapsedTime );
	BOOL CheckPlayerWeaponEnemyIntersection( float fElapsedTime );
};
