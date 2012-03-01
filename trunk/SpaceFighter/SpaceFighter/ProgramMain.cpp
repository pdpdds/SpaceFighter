#include "StdAfx.h"
#include "ProgramMain.h"
#include "SFSystem.h"

CProgramMain::CProgramMain(void)
{
}

CProgramMain::~CProgramMain(void)
{
}

int CProgramMain::Run()
{
	return SFSystem::GetInstance()->Run(640,480,L"Space Fighter");
}

BOOL CProgramMain::Initialize()
{
	SFSystem::GetInstance()->Initialize();

	return 0;
}
