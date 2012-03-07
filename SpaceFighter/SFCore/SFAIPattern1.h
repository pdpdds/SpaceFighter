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
	BOOL m_RotDir;  //회전해야 될 방향.. 이런 이상한 코드가 들어가는 이유는 두 벡터가 이루는 각이 0 ~ 180안의 각에서만 구해지기 때문이다.
	float m_fRotAngle;
	float m_fWeight;
};
