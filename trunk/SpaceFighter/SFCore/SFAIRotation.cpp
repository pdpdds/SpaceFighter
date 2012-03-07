#include "StdAfx.h"
#include "SFAIRotation.h"
#include "GameObject.h"
SFAIRotation::SFAIRotation(void)
: m_fStopTime(0.0f)
{
}

SFAIRotation::~SFAIRotation(void)
{
}

BOOL SFAIRotation::ProcessAI(float fElapsedTime)
{
	GameObject* pOwner = GetOwner();
	D3DXVECTOR3* pPos = pOwner->GetPosition();
	D3DXVECTOR3* pVel = pOwner->GetVelocity();
	D3DXMATRIX* pRot = pOwner->GetRotation();
	
	D3DXQUATERNION qR;													
	D3DXMATRIX mat;			
	
	
	m_fStopTime += fElapsedTime;
			
	BOOL IsResult = TRUE;
	if(m_fStopTime >= (PI * 2))
	{
		m_fStopTime = 0;
		IsResult = FALSE;
	}

	D3DXVECTOR3 vecTrans(0.0f, 0.0f, 0.0f);

	D3DXQuaternionRotationYawPitchRoll(&qR, m_fStopTime, 0, 0);  							
	D3DXMatrixAffineTransformation(&mat, 1.0f, NULL, &qR, &vecTrans);							
	D3DXMatrixInverse( pRot, NULL, &mat);			
	
	return IsResult;
}
