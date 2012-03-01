#include "StdAfx.h"
#include "dxstdafx.h"
#include "Pannel.h"
#include "PictureBoxMgr.h"
#include "TextBoxCtrl.h"

extern CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs

CPannel::CPannel(int PannelID)
{
	m_pPannel = new CDXUTDialog();
	m_PannelID = PannelID;
	m_pPannel->Init( &g_DialogResourceManager );

	m_pPictureBoxMgr = new CPictureBoxMgr();

	m_bEnable = FALSE;
}

CPannel::~CPannel(void)
{
	delete m_pPictureBoxMgr;
	delete m_pPannel;
}

BOOL CPannel::AddPannelComponent(ComponentInfo* pInfo)
{
	switch(pInfo->iComponentType)
	{
	case  GUI_STATIC:
		m_pPannel->AddStatic( pInfo->iIdentifier, pInfo->szGUICaption, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight);
		 m_pPannel->GetControl( pInfo->iIdentifier )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
		break;
	case GUI_BUTTON:
		m_pPannel->AddButton( pInfo->iIdentifier, pInfo->szGUICaption, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight);
		break;
	case GUI_CHECKBOX:
		m_pPannel->AddCheckBox( pInfo->iIdentifier, pInfo->szGUICaption, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight);
		break;
	case GUI_RADIOBUTTON:
		m_pPannel->AddRadioButton( pInfo->iIdentifier, pInfo->iButtonGroup, pInfo->szGUICaption, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight, pInfo->bDefault);
		break;
	case GUI_COMBOBOX:
		{
			CDXUTComboBox *pCombo = NULL;
			m_pPannel->AddComboBox( pInfo->iIdentifier,pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight, L'O', false, &pCombo );
			if( pCombo )
			{

				switch(pInfo->iIdentifier)
				{
				case IDC_GAMEADD_COMBOBOX_GENRE:
					{
						pCombo->SetDropHeight( 100 );
						pCombo->AddItem( L"Action", NULL);
						pCombo->AddItem( L"RPG", NULL);
						pCombo->AddItem( L"Simulation", NULL);
						pCombo->AddItem( L"Adventure", NULL);
						pCombo->AddItem( L"Action Adventure", NULL);
						pCombo->AddItem( L"Puzzle", NULL);
					}
					break;

				case IDC_GAMEADD_COMBOBOX_PLATFORM:
					{
						pCombo->SetDropHeight( 100 );
						pCombo->AddItem( L"PC", NULL);
						pCombo->AddItem( L"PS1", NULL);
						pCombo->AddItem( L"PS2", NULL);
						pCombo->AddItem( L"PS3", NULL);
						pCombo->AddItem( L"XBOX", NULL);
						pCombo->AddItem( L"WII", NULL);
						pCombo->AddItem( L"NDS", NULL);
						pCombo->AddItem( L"PSP", NULL);
						pCombo->AddItem( L"GBA", NULL);
						pCombo->AddItem( L"FM Towns", NULL);
						pCombo->AddItem( L"Nintendo 64", NULL);
						pCombo->AddItem( L"DreamCast", NULL);
						pCombo->AddItem( L"GameCube", NULL);
						pCombo->AddItem( L"MSX", NULL);
						pCombo->AddItem( L"Arcade", NULL);
						pCombo->AddItem( L"iPod", NULL);
						pCombo->AddItem( L"iPad", NULL);
						pCombo->AddItem( L"iPad2", NULL);
					}
					break;
				case IDC_GAMEADD_COMBOBOX_RELEASE:
					{
						pCombo->SetDropHeight( 100 );
						pCombo->AddItem( L"2011", NULL);
						pCombo->AddItem( L"2010", NULL);
						pCombo->AddItem( L"2009", NULL);
						pCombo->AddItem( L"2008", NULL);
						pCombo->AddItem( L"2007", NULL);
						pCombo->AddItem( L"2006", NULL);
						pCombo->AddItem( L"2005", NULL);
						pCombo->AddItem( L"2004", NULL);
						pCombo->AddItem( L"2003", NULL);
						pCombo->AddItem( L"2002", NULL);
						pCombo->AddItem( L"2001", NULL);
						pCombo->AddItem( L"2000", NULL);
						pCombo->AddItem( L"1999", NULL);
						pCombo->AddItem( L"1998", NULL);
						pCombo->AddItem( L"1997", NULL);
						pCombo->AddItem( L"1996", NULL);
						pCombo->AddItem( L"1995", NULL);
						pCombo->AddItem( L"1994", NULL);
						pCombo->AddItem( L"1993", NULL);
						pCombo->AddItem( L"1992", NULL);
						pCombo->AddItem( L"1991", NULL);
						pCombo->AddItem( L"1990", NULL);
						pCombo->AddItem( L"1989", NULL);
						pCombo->AddItem( L"1988", NULL);
						pCombo->AddItem( L"1987", NULL);
						pCombo->AddItem( L"1986", NULL);
						pCombo->AddItem( L"1985", NULL);
						pCombo->AddItem( L"1984", NULL);
						pCombo->AddItem( L"1983", NULL);

					}
					break;

				case IDC_GAMEADD_COMBOBOX_PLAY:
					{
						pCombo->SetDropHeight( 100 );
						pCombo->AddItem( L"2011", NULL);
						pCombo->AddItem( L"2010", NULL);
						pCombo->AddItem( L"2009", NULL);
						pCombo->AddItem( L"2008", NULL);
						pCombo->AddItem( L"2007", NULL);
						pCombo->AddItem( L"2006", NULL);
						pCombo->AddItem( L"2005", NULL);
						pCombo->AddItem( L"2004", NULL);
						pCombo->AddItem( L"2003", NULL);
						pCombo->AddItem( L"2002", NULL);
						pCombo->AddItem( L"2001", NULL);
						pCombo->AddItem( L"2000", NULL);
						pCombo->AddItem( L"1999", NULL);
						pCombo->AddItem( L"1998", NULL);
						pCombo->AddItem( L"1997", NULL);
						pCombo->AddItem( L"1996", NULL);
						pCombo->AddItem( L"1995", NULL);
						pCombo->AddItem( L"1994", NULL);
						pCombo->AddItem( L"1993", NULL);
						pCombo->AddItem( L"1992", NULL);
						pCombo->AddItem( L"1991", NULL);
						pCombo->AddItem( L"1990", NULL);
						pCombo->AddItem( L"1989", NULL);
						pCombo->AddItem( L"1988", NULL);
						pCombo->AddItem( L"1987", NULL);
						pCombo->AddItem( L"1986", NULL);
						pCombo->AddItem( L"1985", NULL);
						pCombo->AddItem( L"1984", NULL);
						pCombo->AddItem( L"1983", NULL);
						
					}
					break;
				}
			}
		}
		break;
	case GUI_SLIDER:
		m_pPannel->AddSlider( pInfo->iIdentifier, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight, 0, 100, 50);
		break;
	case GUI_EDITBOX:
		m_pPannel->AddEditBox( pInfo->iIdentifier, pInfo->szGUICaption, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight, 0);
		break;
	case GUI_TEXTBOX:
		{
			
			CDXUTIMEEditBox::InitDefaultElements( m_pPannel );

			CTextBoxCtrl *pTextBox = new CTextBoxCtrl(m_pPannel);
			m_pPannel->AddControl(pTextBox);
			
			/*m_pPannel->SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
			m_pPannel->SetFont( 2, L"Courier New", 16, FW_NORMAL );

			pTextBox->GetElement( 0 )->iFont = 1;
			pTextBox->GetElement( 1 )->iFont = 1;
			pTextBox->GetElement( 9 )->iFont = 1;
			pTextBox->GetElement( 0 )->TextureColor.Init( D3DCOLOR_ARGB( 128, 255, 255, 255 ) );  // Transparent center
			pTextBox->SetBorderWidth( 7 );
			pTextBox->SetTextColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
			pTextBox->SetCaretColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
			pTextBox->SetSelectedTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			pTextBox->SetSelectedBackColor( D3DCOLOR_ARGB( 255, 40, 72, 72 ) );*/

			pTextBox->SetID(pInfo->iIdentifier);
			pTextBox->Init(pInfo->szGUICaption, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight);
		}
		break;

		// IME-enabled edit box
	case GUI_IMEEDITBOX:
		//m_pPannel->AddIMEEditBox( pInfo->iIdentifier, pInfo->szGUICaption, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight);
		m_pPannel->SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
		m_pPannel->SetFont( 2, L"Courier New", 16, FW_NORMAL );
		CDXUTIMEEditBox* pIMEEdit;
		CDXUTIMEEditBox::InitDefaultElements( m_pPannel );
		if( SUCCEEDED( CDXUTIMEEditBox::CreateIMEEditBox( m_pPannel, pInfo->iIdentifier,
			pInfo->szGUICaption,  pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight, false, &pIMEEdit ) ) )
		{
			m_pPannel->AddControl( pIMEEdit );
			pIMEEdit->GetElement( 0 )->iFont = 1;
			pIMEEdit->GetElement( 1 )->iFont = 1;
			pIMEEdit->GetElement( 9 )->iFont = 1;
			pIMEEdit->GetElement( 0 )->TextureColor.Init( D3DCOLOR_ARGB( 128, 255, 255, 255 ) );  // Transparent center
			pIMEEdit->SetBorderWidth( 7 );
			pIMEEdit->SetTextColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
			pIMEEdit->SetCaretColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
			pIMEEdit->SetSelectedTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			pIMEEdit->SetSelectedBackColor( D3DCOLOR_ARGB( 255, 40, 72, 72 ) );
		}

		break;
	case GUI_LISTBOX:
		m_pPannel->AddListBox( pInfo->iIdentifier, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight, 0);

		/*for( int i = 0; i < 1000; ++i )
		{
			WCHAR wszText[50];
			StringCchPrintf( wszText, 50, L"Single-selection listbox item %d", i );
			m_pPannel->GetListBox( pInfo->iIdentifier )->AddItem( wszText, ( LPVOID )( size_t )i );
		}*/
		break;
	//case GUI_Control:
		//m_pPannel->AddControl( pInfo->iIdentifier, pInfo->szGUICaption, pInfo->iPosX, pInfo->iPosY, pInfo->iWidth, pInfo->iHeight);
		//break;

	case GUI_IMAGE:
		m_pPictureBoxMgr->AddPictureBox(pInfo);
		break;

	default:
		return FALSE;

	}

	return TRUE;
}

BOOL CPannel::OnRender(float fElapsedTime)
{
	m_pPannel->OnRender(fElapsedTime);

	m_pPictureBoxMgr->OnRender(fElapsedTime);

	return TRUE;
}

BOOL CPannel::OnResetDevice()
{
	m_pPictureBoxMgr->OnResetDevice();

	return TRUE;
}

bool CPannel::ProcessMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_pPannel->MsgProc( hWnd, uMsg, wParam, lParam );
}