#include "StdAfx.h"
#include "SFScene2.h"
#include "SFBackGroundWater.h"

SFScene2::SFScene2(void)
{
}

SFScene2::~SFScene2(void)
{
}

BOOL SFScene2::Init()
{
	SFBackGround* pBackGround = new SFBackGroundWater();
	pBackGround->Init();

	AddBackGround(pBackGround);

	return TRUE;
}
