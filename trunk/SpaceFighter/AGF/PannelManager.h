#pragma once
#include "dxstdafx.h"

class CPannel;

class CPannelManager
{
	typedef std::map<int, CPannel*> PannelMap;
public:
	CPannelManager(void);
	virtual ~CPannelManager(void);

	BOOL CreatePannel(CPannel* pPannel);
	BOOL CreatePannelComponent(ComponentInfo* pinfo);
	CPannel* FindPannel(int iPannelID);

	BOOL StartFirstAction(int iIndex = -1);
	BOOL AddGameList(CDXUTListBox* pListBox);
	int SearchGame(CDXUTListBox* pListBox, CDXUTListBox* pListBoxSearchResult, CONST TCHAR* szFileName);
	BOOL FileCopy(const TCHAR* szSrcPath, const TCHAR* szDestPath);

	BOOL OnRender(float fElapsedTime);
	BOOL OnResetDevice();
	BOOL ProcessMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL TurnOffAllPannel();

	int m_iBeforePannel;

protected:

private:
	PannelMap m_PannelMap;

};
