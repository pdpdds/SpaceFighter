#pragma once
#include "SFAIPattern.h"

class SFAIPattern1 : public SFAIPattern
{
	enum ePatternStep
	{
		eStep_1 = 0,
		eStep_2,
		eStep_3,
		eStep_4,
	};
public:
	SFAIPattern1(void);
	virtual ~SFAIPattern1(void);

	virtual BOOL ProcessAI(float fElapsedTime) override;

protected:

private:
	ePatternStep m_ePatternStep; 
	BOOL m_RotDir;  //ȸ���ؾ� �� ����.. �̷� �̻��� �ڵ尡 ���� ������ �� ���Ͱ� �̷�� ���� 0 ~ 180���� �������� �������� �����̴�.
	float m_fRotAngle;
	float m_fWeight;
};
