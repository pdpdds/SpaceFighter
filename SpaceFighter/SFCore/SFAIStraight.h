#pragma once
#include "SFAIPattern.h"

class SFAIStraight : public SFAIPattern
{
public:
	SFAIStraight(void);
	virtual ~SFAIStraight(void);

	virtual BOOL ProcessAI(float fElapsedTime) override;

protected:

private:
};
