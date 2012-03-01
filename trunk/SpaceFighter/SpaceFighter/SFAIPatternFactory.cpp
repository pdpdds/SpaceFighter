#include "StdAfx.h"
#include "SFAIPatternFactory.h"
#include "SFAIPattern1.h"
#include "SFAIStraight.h"

SFAIPatternFactory::SFAIPatternFactory(void)
{
}

SFAIPatternFactory::~SFAIPatternFactory(void)
{
}

SFAIPattern* SFAIPatternFactory::CreateAIPattern( char* szPatternName )
{
	if(0 == strcmp("AIPattern1", szPatternName))
	{
		return new SFAIPattern1();
	}
	if(0 == strcmp("AIStraight", szPatternName))
	{
		return new SFAIStraight();
	}

	return NULL;
}