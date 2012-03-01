#include "StdAfx.h"
#include "TextScrollManager.h"
#include "DirectXSystem.h"
#include "strsafe.h"

using namespace std;
CTextScrollManager::CTextScrollManager(void)
{
	m_fTextStartPosY = 0;
	m_fVerticalScrollVelocity = 0;

	m_bScrolling = FALSE;

	m_iTextIndex = 0;
}

CTextScrollManager::~CTextScrollManager(void)
{
}

BOOL CTextScrollManager::OnRender( float fElapsedTime )
{
	RECT rc;
	SetRect( &rc, 0, (int)m_fTextStartPosY, CDirectXSystem::GetInstance()->GetWidth(), 0 );
	int iPosYDelta = 30;

	mapText::iterator iter = m_mapText.begin();

	for(iter; iter != m_mapText.end(); iter++)
	{
		TCHAR* szText = (TCHAR*)(iter->second.c_str());

		m_Font.Print(szText, rc, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));

		rc.top += iPosYDelta;
	}

	return TRUE;
}

BOOL CTextScrollManager::OnResetDevice()
{
	m_Font.OnResetDevice();	

	return TRUE;
}

BOOL CTextScrollManager::OnFrameMove( double fTime, float fElapsedTime )
{
	if(m_mapText.size() == 0)
	{
		m_bScrolling = FALSE;
		return FALSE;
	}

	if(m_bScrolling == TRUE)
	{		
		m_fTextStartPosY -= m_fVerticalScrollVelocity * fElapsedTime;


		while(m_fTextStartPosY < -30)
		{
			
			mapText::iterator iter = m_mapText.begin();

			if(iter != m_mapText.end())
				m_mapText.erase(iter);
			else
			{
				break;
			}

			m_fTextStartPosY += 30;
		}


	}

	return TRUE;
}

BOOL CTextScrollManager::OnLostDevice()
{
	m_Font.OnLostDevice();

	return TRUE;
}

BOOL CTextScrollManager::OnDestroyDevice()
{

	return TRUE;
}

BOOL CTextScrollManager::Initialize(RECT& ScrollArea)
{
	m_ScrollArea = ScrollArea;

	m_fTextStartPosY = ScrollArea.bottom;
	m_fVerticalScrollVelocity = 30.0f;

	m_bScrolling = FALSE;

	D3DXFONT_DESC desc;
	desc.CharSet = HANGEUL_CHARSET;
	desc.Height = 20;
	desc.Width = 10;
	desc.Weight = 100;
	desc.Quality = ANTIALIASED_QUALITY;
	desc.MipLevels = 1;
	desc.Italic = 0;
	desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	desc.PitchAndFamily = FF_DONTCARE;
	_tcscpy_s(desc.FaceName, _T("HY견명조"));

	m_Font.Initialize(&desc);

	return TRUE;
}

BOOL CTextScrollManager::AddText( TCHAR* szText )
{	
	m_iTextIndex++;
	std::wstring szStr = szText;

	m_mapText.insert(mapText::value_type(m_iTextIndex, szStr));

	return TRUE;
}

BOOL CTextScrollManager::LoadTextScrollData( TCHAR* szGameName )
{
	m_fTextStartPosY = m_ScrollArea.bottom;
	m_mapText.clear();

	
	std::wifstream fin;

	WCHAR wFileName[MAX_PATH];
	StringCchPrintf( wFileName, MAX_PATH, L"./GameList/%s/memo.txt", szGameName);

	fin.open(wFileName, ios_base::in);
	fin.imbue(locale("korean"));

	//TCHAR szText[MAX_PATH];
	//memset(szText, 0, sizeof(TCHAR) * MAX_PATH);
	
	//fin.getline(szText, MAX_PATH);
	
	wstring line;
	while (getline(fin, line))
	{
		/*wstringstream linestream(line);

		wstring name;
		getline(linestream, name, L'/');

		wstring content;
		getline(linestream, content);
		*/

		AddText((TCHAR*)line.c_str());
	}

	fin.close();

	SetEnable(TRUE);

	return TRUE;

}

BOOL CTextScrollManager::LoadTextScrollDataFromMemory( TCHAR* szGameName )
{
	m_fTextStartPosY = m_ScrollArea.bottom;
	m_mapText.clear();

	
	std::wifstream fin;

	WCHAR wFileName[MAX_PATH];
	StringCchPrintf( wFileName, MAX_PATH, L"./GameList/%s/memo.txt", szGameName);

	wchar_t buffer[] = L"한글도 될까\nHello World!\nThis is next line\nThe last line";       
	membuf mb(buffer, sizeof(buffer)/sizeof(buffer[0]));      
	wistream istr(&mb);

	wstring line;
	while (getline(istr, line))
	{
		/*wstringstream linestream(line);

		wstring name;
		getline(linestream, name, L'/');

		wstring content;
		getline(linestream, content);
		*/

		AddText((TCHAR*)line.c_str());
	}

	fin.close();

	SetEnable(TRUE);

	return TRUE;

}