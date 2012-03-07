#pragma once
#include "SFAIPattern.h"

class SFAITrace : public SFAIPattern
{
public:
	SFAITrace(void);
	virtual ~SFAITrace(void);

	virtual BOOL ProcessAI(float fElapsedTime) override;

protected:

private:
	float m_fWeight;
};
