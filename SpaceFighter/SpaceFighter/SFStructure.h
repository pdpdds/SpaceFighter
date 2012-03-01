#pragma once


typedef struct tag_WeaponInfo
{
	int WeaponDelay; //무기 발사후 다음 발사시까지의 딜레이
	int MaxFireObjectCount; //한 장면에서 존재할 수 있는 최대 무기 발사체의 수
	DWORD FireTime; //무기발사 시간
	int ResourceID; //무기 리소스 아이디
	D3DXVECTOR3 vecSize; //무기의 사이즈
	D3DXVECTOR3	vecVelocity; //무기의 속도
	D3DXVECTOR3 vecStartPos; //최초 발사 위치
	int Damage; //무기의 위력
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

