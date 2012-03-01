#include "StdAfx.h"
#include "SFAIPattern1.h"
#include "GameObject.h"

SFAIPattern1::SFAIPattern1(void)
: m_ePatternStep(eStep_1)
, m_RotDir(FALSE)
, m_fRotAngle(0.0f)
, m_fWeight(0.0f)
{
}

SFAIPattern1::~SFAIPattern1(void)
{
}

BOOL SFAIPattern1::ProcessAI( float fElapsedTime )
{
	GameObject* pOwner = GetOwner();
	D3DXVECTOR3* pPos = pOwner->GetPosition();
	D3DXVECTOR3* pVel = pOwner->GetVelocity();
	D3DXMATRIX* pRot = pOwner->GetRotation();

	D3DXQUATERNION qR;													
	D3DXMATRIX mat;	

	
	if(eStep_1 == m_ePatternStep)
	{
		pPos->z += pVel->z * fElapsedTime;

		if(pPos->z <= 70.0f)
		{
			pPos->z = 70.0f;
			m_ePatternStep = eStep_2;
		}

		return TRUE;
	}
	else if(eStep_2 == m_ePatternStep) //현재 순간에서의 주인공 비행기와 자신과의 각도(0,0,-1)를 구한다.
	{
		GameObject* pTarget = pOwner->GetTarget();

		if(NULL == pTarget)
			return FALSE;	

		D3DXVECTOR3 vecTrace;
		D3DXVECTOR3 vecOut;

		vecTrace.x = pTarget->GetPosition()->x - pPos->x;
		vecTrace.y = 0;
		vecTrace.z = pTarget->GetPosition()->z - pPos->z;

		D3DXVec3Normalize(&vecOut, const_cast<D3DXVECTOR3*>(&vecTrace));

		*pVel = vecOut * 120.0f;

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
			
		m_fRotAngle = radian;

		if(fRadian2 > fRadian1)
		{
			

			if(fRadian2 - fRadian1 < PI) 
			{
				m_RotDir = TRUE;

			}
			else
			{
				m_RotDir = FALSE;
			}
		}
		else 
		{
			if(fRadian1 - fRadian2  < PI) 
			{

				m_RotDir = FALSE;
			}
			else
			{
				m_RotDir = TRUE;
			}
		}
	
		m_ePatternStep = eStep_3;

		return TRUE;
	}
	else if(eStep_3 == m_ePatternStep)
	{
		D3DXVECTOR3 vecTrans(0.0f, 0.0f, 0.0f);
		
		D3DXQuaternionRotationYawPitchRoll(&qR, m_fWeight, 0, 0);  	

		D3DXMatrixAffineTransformation(&mat, 1.0f, NULL, &qR, &vecTrans);							
		D3DXMatrixInverse( pRot, NULL, &mat);

		if(m_RotDir == TRUE)
		{
			m_fWeight += fElapsedTime;
		}
		else
		{
			m_fWeight -= fElapsedTime;
		}

		if(m_fRotAngle - fabsf(m_fWeight) < 0.0f)
			m_ePatternStep = eStep_4;

		return TRUE;
	}
	else if(eStep_4 == m_ePatternStep)
	{
		GameObject* pOwner = GetOwner();
		D3DXVECTOR3* pPos = pOwner->GetPosition();
		D3DXVECTOR3* pVel = pOwner->GetVelocity();
		*pPos += (*pVel) * fElapsedTime;

		return TRUE;
	}

	return FALSE;
}