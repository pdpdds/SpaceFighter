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

	BOOL CheckIntersection(GameObject* pObject); //�ٸ� ������Ʈ�� �浹 ���θ� �˻��Ѵ�.
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
	unsigned int m_ObjectType; //�̻�������, ��������� ���
	unsigned int m_ObjectID;   //������Ʈ ���� ��ȣ

	D3DXVECTOR3 m_Position;   //��ġ
	D3DXVECTOR3 m_Velocity;   //�ӵ�
	D3DXMATRIX  m_Rotation;   //ȸ��
	
	unsigned int	m_Health;  //ü��

	BOOL m_bInteract; //��ȣ�ۿ� ������ ��������
	BOOL m_bDisable; //���忡�� �����Ѵ�.

	GameObject* m_pTarget; //Ư�� ������Ʈ�� Ÿ�����ϰ� �ִ���
	SFRes*	m_pSFRes; //�ش� ������Ʈ�� ���ҽ��� ���� ������

	OBB* m_pOBB;  //�浹�ڽ� ����

	DWORD m_Timeline;

	float m_Scale;
};
