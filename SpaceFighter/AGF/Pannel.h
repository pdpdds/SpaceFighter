#pragma once

class CDXUTDialog;
class CPictureBoxMgr;

class CPannel
{
public:
	CPannel(int PannelID);
	virtual ~CPannel(void);

	BOOL AddPannelComponent(ComponentInfo* pInfo);
	BOOL OnRender(float fElapsedTime);
	BOOL OnResetDevice();
	bool ProcessMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int GetPannelID(){return m_PannelID;}
	CDXUTDialog* GetPannelControl(){return m_pPannel;}
	CPictureBoxMgr* GetPictureBoxMgr(){return m_pPictureBoxMgr;}

	void SetEnable(BOOL bEnable){m_bEnable = bEnable;}
	BOOL GetEnable(){return m_bEnable;}

protected:

private:
	CDXUTDialog*  m_pPannel;
	CPictureBoxMgr*  m_pPictureBoxMgr;	
	int m_PannelID;
	BOOL m_bEnable;
};
