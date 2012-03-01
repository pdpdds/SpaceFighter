#include "StdAfx.h"
#include "PannelManager.h"
#include "Pannel.h"
#include "PannelCallback.h"
#include "PictureBoxMgr.h"
#include "Ini.h"
CPannelManager::CPannelManager(void)
{
	m_iBeforePannel = PANNEL_GAMEINFO;
}

CPannelManager::~CPannelManager(void)
{
}

BOOL CPannelManager::CreatePannel(CPannel* pPannel)
{	
	m_PannelMap.insert(PannelMap::value_type(pPannel->GetPannelID(), pPannel));

	switch(pPannel->GetPannelID())
	{
	case PANNEL_MENU:
		{
			pPannel->GetPannelControl()->SetCallback(OnPannelMenuEvent);
		}
		break;
	case PANNEL_GAMEINFO:
		{
			pPannel->GetPannelControl()->SetCallback(OnPannelGameInfoEvent);
		}
		break;
	case PANNEL_SEARCHRESULT:
		{
			pPannel->GetPannelControl()->SetCallback(OnPannelSearchResultEvent);
		}
		break;
	case PANNEL_GAMEADD:
		{
			pPannel->GetPannelControl()->SetCallback(OnPannelGameAddEvent);
		}
		break;
	/*case PANNEL_GAMEPRESENTATION:
		{
			pPannel->GetPannelControl()->SetCallback(OnPannelGamePresentEvent);
		}
		break;*/	

	default:
		return FALSE;
	}

	return TRUE;
}

BOOL CPannelManager::CreatePannelComponent(ComponentInfo* pinfo)
{	
	PannelMap::iterator iter = m_PannelMap.find(pinfo->PannelID);

	if(iter == m_PannelMap.end())
		return FALSE;

	CPannel* pPannel = (CPannel*)iter->second;

	pPannel->AddPannelComponent(pinfo);

	return TRUE;
}

CPannel* CPannelManager::FindPannel(int iPannelID)
{
	PannelMap::iterator iter = m_PannelMap.find(iPannelID);

	if(iter == m_PannelMap.end())
		return NULL;

	return(CPannel*)iter->second;
}

BOOL CPannelManager::OnRender(float fElapsedTime)
{
	PannelMap::iterator iter = m_PannelMap.begin();

	for(iter; iter != m_PannelMap.end(); iter++)
	{
		CPannel* pPannel = (CPannel*)(iter->second);

		if(pPannel->GetEnable() == TRUE)
			pPannel->OnRender(fElapsedTime);
	}

	return TRUE;
}

BOOL CPannelManager::OnResetDevice()
{
	PannelMap::iterator iter = m_PannelMap.begin();

	for(iter; iter != m_PannelMap.end(); iter++)
	{
		CPannel* pPannel = (CPannel*)(iter->second);
		pPannel->OnResetDevice();
	}

	return TRUE;
}


BOOL CPannelManager::ProcessMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PannelMap::iterator iter = m_PannelMap.begin();

	for(iter; iter != m_PannelMap.end(); iter++)
	{
		CPannel* pPannel = (CPannel*)(iter->second);

		if(pPannel->GetEnable() == TRUE)
			pPannel->ProcessMessage( hWnd, uMsg, wParam, lParam );
	}
	
	return TRUE;
}

BOOL CPannelManager::TurnOffAllPannel()
{
	PannelMap::iterator iter = m_PannelMap.begin();

	for(iter; iter != m_PannelMap.end(); iter++)
	{
		CPannel* pPannel = (CPannel*)(iter->second);

		pPannel->SetEnable(FALSE);
	}
	
	return TRUE;
}



BOOL CPannelManager::StartFirstAction(int iIndex)
{	
	PannelMap::iterator iter = m_PannelMap.find(m_iBeforePannel);

	if(iter == m_PannelMap.end())
		return FALSE;

	CPannel* pPannel = (CPannel*)iter->second;

	pPannel->SetEnable(TRUE);

	CDXUTListBox* pListBox = NULL;

	if(m_iBeforePannel == PANNEL_GAMEINFO)
		pListBox = (CDXUTListBox*)(pPannel->GetPannelControl()->GetControl(IDC_LISTBOX_GAMELISTING));


	if(m_iBeforePannel == PANNEL_SEARCHRESULT)
		pListBox = (CDXUTListBox*)(pPannel->GetPannelControl()->GetControl(IDC_SEARCH_LISTBOX_GAMELISTING));


	if(m_iBeforePannel == PANNEL_GAMEINFO)
	{
		pListBox->RemoveAllItems();
		AddGameList(pListBox);

		CDXUTListBox* pListBox = pPannel->GetPannelControl()->GetListBox(IDC_LISTBOX_GAMELISTING);

		DXUTListBoxItem* pItem = pListBox->GetItem(
			pListBox->GetSelectedIndex( -1 ) );


		if(NULL == pItem)
			pItem =pListBox->GetItem(0);

		if(pItem != NULL)
		{
			CIni ini;
			TCHAR szGameName[100];
			TCHAR szGameGenre[100];
			TCHAR szGamePlatform[100];
			TCHAR szGameRelease[100];
			TCHAR szGamePlay[100];

			WCHAR wFileName[MAX_PATH];
			StringCchPrintf( wFileName, MAX_PATH, L"./GameList/%s/Description.txt", pItem->strText);
			TCHAR szNewPictureName[MAX_PATH];
			StringCchPrintf( szNewPictureName, MAX_PATH, L"./GameList/%s/Title.jpg", pItem->strText);

			ini.SetPathName(wFileName);
			ini.GetString(L"GameInfo",L"NAME",szGameName, 100);
			ini.GetString(L"GameInfo",L"GENRE",szGameGenre, 100);
			ini.GetString(L"GameInfo",L"PLATFORM",szGamePlatform, 100);
			ini.GetString(L"GameInfo",L"RELEASE",szGameRelease, 100);
			ini.GetString(L"GameInfo",L"PLAY",szGamePlay, 100);


			pPannel->GetPannelControl()->GetStatic(IDC_STATIC_GAMENAME_TXT)->SetText(szGameName);
			pPannel->GetPannelControl()->GetStatic(IDC_STATIC_GAMEGENRE_TXT)->SetText(szGameGenre);
			pPannel->GetPannelControl()->GetStatic(IDC_STATIC_GAMEPLATFORM_TXT)->SetText(szGamePlatform);
			pPannel->GetPannelControl()->GetStatic(IDC_STATIC_GAMERELEASEYEAR_TXT)->SetText(szGameRelease);
			pPannel->GetPannelControl()->GetStatic(IDC_STATIC_GAMEPLAYYEAR_TXT)->SetText(szGamePlay);

			pPannel->GetPictureBoxMgr()->ChangeNewPictureBox(IDC_PICTUREBOX_GAMETITLE, szNewPictureName);
		}
	}

	if(iIndex >= 0)
		pPannel->OnResetDevice();

	iter = m_PannelMap.find(PANNEL_MENU);

	if(iter == m_PannelMap.end())
		return FALSE;

	pPannel = (CPannel*)iter->second;
	pPannel->SetEnable(TRUE);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////


BOOL CPannelManager::AddGameList(CDXUTListBox* pListBox)
{
	HANDLE File_Hwnd;	
	WIN32_FIND_DATA wfd;

	BOOL bResult = TRUE;

	File_Hwnd=FindFirstFile(L".\\GameList\\*.*",&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult = FALSE;
	else bResult = TRUE;

	while (bResult)
	{
		if(FILE_ATTRIBUTE_DIRECTORY & wfd.dwFileAttributes)
		{
			if(_tcscmp(wfd.cFileName, _T(".")) != 0 && _tcscmp(wfd.cFileName, _T("..")) != 0)
				pListBox->AddItem( wfd.cFileName, (LPVOID)(size_t)pListBox->GetSize() );	
		}
		bResult = FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);

	return TRUE;
}

TCHAR *_tcsstri(CONST TCHAR* szString,CONST TCHAR* szSubString)
{
     CONST TCHAR *s,*sub;

     for (;*szString;szString++) 
	 {
          for (sub=szSubString, s = szString; *sub && *s; sub++,s++) 
		  {

              if (_totlower(*s) != _totlower(*sub)) break;

          }

          if (*sub == 0) return (TCHAR *)szString;

     }

     return NULL;
}



int CPannelManager::SearchGame(CDXUTListBox* pListBox, CDXUTListBox* pListBoxSearchResult, CONST TCHAR* szFileName)
{

/*	while( ( nSelected = ((CDXUTListBox *)pControl)->GetSelectedIndex( nSelected ) ) != -1 )
                    {
                        StringCchPrintf( wszOutput + lstrlenW( wszOutput ), 1024 - lstrlenW( wszOutput ), L"%d,", nSelected );
                    }
*/

	pListBoxSearchResult->RemoveAllItems();

	int Count = 0;
	for(int i = 0;  i < pListBox->GetSize(); i++)
	{
		DXUTListBoxItem* pItem = pListBox->GetItem(i);
		
		if(NULL != _tcsstri(pItem->strText, szFileName))
		{
			pListBoxSearchResult->AddItem(pItem->strText, (LPVOID)(size_t)Count);
			Count++;
		}
	}


	return Count;
}

BOOL CPannelManager::FileCopy(const TCHAR* szSrcPath, const TCHAR* szDestPath) 
{
	FILE *in, *out;
	char* buf;
	size_t len;
	errno_t err;

	if (!_tcsicmp(szSrcPath, szDestPath))
		return FALSE;

	if ((err  = _tfopen_s(&in, szSrcPath, _T("rb"))) != 0) 
		return FALSE;

	if ((err = _tfopen_s(&out, szDestPath, _T("wb"))) != 0) 
	{ 
		fclose(in); 
		return FALSE; 
	}

	if ((buf = (char *) malloc(1000)) == NULL) 
	{ 
		fclose(in); 
		fclose(out); 
		return FALSE;
	}

	while ( (len = fread(buf, sizeof(char), sizeof(buf), in)) != NULL )
	{
		if (fwrite(buf, sizeof(char), len, out) == 0) 
		{
			fclose(in); fclose(out);
			free(buf);
			_tunlink(szDestPath);
			return FALSE;
		}
	}

	fclose(in);
	fclose(out);
	free(buf); 
	return TRUE;
}




