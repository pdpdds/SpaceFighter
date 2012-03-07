#include "StdAfx.h"
#include "ProgramMain.h"
#include "SFSystem.h"
#include "EHModuleManager.h"

CEHModuleManager ModuleManger;

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
	//if(FALSE == ModuleManger.Install(DL_MY_HANDLER_STACKOVERFLOW, EH_BUGTRAP))
	if(FALSE == ModuleManger.Install(DL_MY_HANDLER_STACKOVERFLOW, EH_BUGTRAP_MIX))

	{
		printf("���� �ڵ鷯 ��ġ ����\n");
	}


	SFSystem::GetInstance()->Initialize();

	return 0;
}
