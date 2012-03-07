// SpaceFighter.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SpaceFighter.h"
#include "ProgramMain.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	CProgramMain* pMain = new CProgramMain();
	pMain->Initialize();

	int iRet = pMain->Run();

	delete pMain;

	return iRet;
}