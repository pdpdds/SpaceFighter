#pragma once


typedef struct tag_WeaponInfo
{
	int WeaponDelay; //���� �߻��� ���� �߻�ñ����� ������
	int MaxFireObjectCount; //�� ��鿡�� ������ �� �ִ� �ִ� ���� �߻�ü�� ��
	DWORD FireTime; //����߻� �ð�
	int ResourceID; //���� ���ҽ� ���̵�
	D3DXVECTOR3 vecSize; //������ ������
	D3DXVECTOR3	vecVelocity; //������ �ӵ�
	D3DXVECTOR3 vecStartPos; //���� �߻� ��ġ
	int Damage; //������ ����
}WeaponInfo;

typedef struct tag_GameObjectDesc
{
	int ObjectType;
	int ResID;
	D3DXVECTOR3 vecStartPos;
	D3DXVECTOR3 vecVel;
	DWORD TimeLine;
	char szAIPatternName[MAX_PATH];
}GameObjectDesc;

typedef struct tag_SFMessage
{
	int MessageID;
	
}SFMessage;

