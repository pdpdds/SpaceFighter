#include "StdAfx.h"
#include "SFAITrace.h"
#include "GameObject.h"
#include "SFSystem.h"

SFAITrace::SFAITrace(void)
: m_fWeight(0.0f)
{
}

SFAITrace::~SFAITrace(void)
{
}

BOOL SFAITrace::ProcessAI( float fElapsedTime )
{
	GameObject* pOwner = GetOwner();
	D3DXVECTOR3* pPos = pOwner->GetPosition();
	D3DXVECTOR3* pVel = pOwner->GetVelocity();
	D3DXMATRIX* pRot = pOwner->GetRotation();
	GameObject* pTarget = pOwner->GetTarget();

	if(NULL == pTarget)
		return FALSE;	

	D3DXVECTOR3 vecTrace;
	D3DXVECTOR3 vecOut;

	vecTrace.x = pTarget->GetPosition()->x - pPos->x;
	vecTrace.y = 0;
	vecTrace.z = pTarget->GetPosition()->z - pPos->z;

	D3DXVec3Normalize(&vecOut, const_cast<D3DXVECTOR3*>(&vecTrace));

	*pVel = vecOut * 18.0f;

	*pPos = (*pPos) + ((*pVel) * fElapsedTime); 

	D3DXVECTOR3 vecTrans(0.0f, 0.0f, 0.0f);

	D3DXQUATERNION qR;													
	D3DXMATRIX mat;		

	D3DXVECTOR3 vec(0,0,-1);
	D3DXVec3TransformNormal( &vec, &vec, pRot);
	D3DXVec3Normalize(&vec, &vec);

	float fValue = D3DXVec3Dot(&vec, &vecOut);
	float radian = acosf(fValue);

	D3DXVECTOR3 defaultVec(0,0,-1);
	float fRadian1 = acosf(D3DXVec3Dot(&defaultVec, &vec));
	float fRadian2 = acosf(D3DXVec3Dot(&defaultVec, &vecOut));

	if(vec.x < 0)
		fRadian1 = 2 * PI - fRadian1;

	if(vecOut.x < 0)
		fRadian2 = 2 * PI - fRadian2;

	if(fabsf(fRadian2 - fRadian1) > 0.1f)
	if(fRadian2 > fRadian1)
	{
		if(fRadian2 - fRadian1 < PI) 
		{
				m_fWeight += fElapsedTime;

		}
		else
		{
			
				m_fWeight -= fElapsedTime;
		}
	}
	else 
	{
		if(fRadian1 - fRadian2  < PI) 
		{
			
				m_fWeight -= fElapsedTime;

		}
		else
		{
				m_fWeight += fElapsedTime;
		}
	}
	

	

	/*if(m_fWeight > radian)
		m_fWeight = radian;

	float fResult = 0.0f;
	if(radian > PI)
		fResult = m_fWeight - 2 * PI;
	else
		fResult = m_fWeight;*/

	D3DXQuaternionRotationYawPitchRoll(&qR, m_fWeight, 0, 0);  	

	D3DXMatrixAffineTransformation(&mat, 1.0f, NULL, &qR, &vecTrans);							
	D3DXMatrixInverse( pRot, NULL, &mat);

	float fDistance = vecTrace.x * vecTrace.x + vecTrace.y * vecTrace.y + vecTrace.z * vecTrace.z;

	fDistance = sqrtf(fDistance);

	if(fDistance < 0.1f)
	{
		return FALSE;
	}

	return TRUE;
}