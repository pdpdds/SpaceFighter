#pragma once

class SFAIPattern;

class SFAIPatternFactory
{
public:
	SFAIPatternFactory(void);
	virtual ~SFAIPatternFactory(void);

	static SFAIPattern* CreateAIPattern(char* szPatternName);
};
