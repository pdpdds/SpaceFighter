#pragma once
#include "SFRes.h"
#include "SFResMgr.h"
#include "OBB.h"
#include "SFMessageReceiver.h"

class GameObject : public SFMessageReceiver
{
public:
	GameObject(void);
	virtual ~GameObject(void);

	GameObject* GetTarget(){return m_pTarget;}
	void SetTarget(GameObject* pTarget){m_pTarget = pTarget;}

	unsigned int GetObjectType(){return m_ObjectType;}
	void SetObjectType(unsigned int ObjectType){m_ObjectType = ObjectType;}

	D3DXVECTOR3* GetPosition(){return &m_Position;}
	void SetPosition(D3DXVECTOR3& Pos){m_Position = Pos;}

	D3DXVECTOR3* GetVelocity(){return &m_Velocity;}
	void SetVelocity(D3DXVECTOR3& Vel){m_Velocity = Vel;}

	D3DXMATRIX* GetRotation(){return &m_Rotation;}
	void SetRotation(D3DXMATRIX& mat){m_Rotation = mat;}

	virtual BOOL Render(float fElapsedTime) {return FALSE;}
	virtual BOOL Update(float fElapsedTime) {return FALSE;}

	virtual BOOL ProcessInput(float fElapsedTime){return FALSE;}

	void SetDisable(BOOL bDisable){m_bDisable = bDisable;}
	BOOL GetDisable(){return m_bDisable;}

	SFRes* GetResource(){return m_pSFRes;}
	void SetResource(SFRes* pRes){m_pSFRes = pRes;}

	BOOL CheckIntersection(GameObject* pObject); //다른 오브젝트와 충돌 여부를 검사한다.
	virtual BOOL ProcessInteraction(GameObject* pObject){return TRUE;}
	virtual BOOL ProcessAttatcked(GameObject* pAttackObject){return TRUE;}

	OBB* GetOBB(){return m_pOBB;}
	void SetOBB(OBB* pOBB){m_pOBB = pOBB;}

	void SetTimeLine(DWORD TimeLine){m_Timeline = TimeLine;}
	DWORD GetTimeLine(){return m_Timeline;}

	void SetScale(float Scale){m_Scale = Scale;}
	float GetScale(){return m_Scale;}

	void SetInteract(BOOL bInteract){m_bInteract = bInteract;}
	BOOL GetInteract(){return m_bInteract;}

	int GetHealth(){return m_Health;}

protected:

private:
	unsigned int m_ObjectType; //미사일인지, 비행기인지 등등
	unsigned int m_ObjectID;   //오브젝트 고유 번호

	D3DXVECTOR3 m_Position;   //위치
	D3DXVECTOR3 m_Velocity;   //속도
	D3DXMATRIX  m_Rotation;   //회전
	
	unsigned int	m_Health;  //체력

	BOOL m_bInteract; //상호작용 가능한 상태인지
	BOOL m_bDisable; //월드에서 제거한다.

	GameObject* m_pTarget; //특정 오브젝트를 타겟팅하고 있는지
	SFRes*	m_pSFRes; //해당 오브젝트의 리소스에 관한 포인터

	OBB* m_pOBB;  //충돌박스 정보

	DWORD m_Timeline;

	float m_Scale;
};
