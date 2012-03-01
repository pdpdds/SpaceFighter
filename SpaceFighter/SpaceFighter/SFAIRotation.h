#pragma once
#include "SFAIPattern.h"

class SFAIRotation : public SFAIPattern
{
public:
	SFAIRotation(void);
	virtual ~SFAIRotation(void);

	virtual BOOL ProcessAI(float fElapsedTime) override;

protected:

private:
	float m_fStopTime;
};
