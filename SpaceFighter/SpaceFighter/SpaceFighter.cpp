// SpaceFighter.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SpaceFighter.h"
#include "EHModuleManager.h"
#include "ProgramMain.h"

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "strmbasd.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")

#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif

#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "d3dxof.lib")

#pragma comment(lib, "lua.lib")
#pragma comment(lib, "OrangeDump.lib")
#pragma comment(lib, "OrangeIni.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	CEHModuleManager ModuleManger;
	//if(FALSE == ModuleManger.Install(DL_MY_HANDLER_STACKOVERFLOW, EH_BUGTRAP))
	if(FALSE == ModuleManger.Install(DL_MY_HANDLER_STACKOVERFLOW, EH_BUGTRAP_MIX))
	
	{
		printf("예외 핸들러 설치 실패\n");
	}

	CProgramMain* pMain = new CProgramMain();
	pMain->Initialize();

	int iRet = pMain->Run();

	delete pMain;

	return iRet;
}