#pragma once
#include "d3d9.h"
#include "d3d9types.h"
#include "d3dx9math.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//String Size
///////////////////////////////////////////////////////////////////////////////////////////////////
#define LEN_GUI_CAPTION			100
#define LEN_GAME_NAME			256

typedef struct tagComponentInfo
{
	int PannelID;
	int iPosX;
	int iPosY;
	int iHeight;
	int iWidth;
	int iComponentType;
	int iIdentifier;
	TCHAR szGUICaption[LEN_GUI_CAPTION];

/////////////////////////////////////////////////////
//For Radio Button
/////////////////////////////////////////////////////
	int iButtonGroup;
	bool bDefault;

	tagComponentInfo()
	{
		PannelID = -1;
		iComponentType = -1;
		iIdentifier = -1;
		iPosX = -1;
		iPosY = -1;
		iHeight = -1;
		iWidth = -1;
		memset(szGUICaption, 0, sizeof(TCHAR) * LEN_GUI_CAPTION);

		iButtonGroup = -1;
		bDefault = false;
	}

}ComponentInfo;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct CUSTOMVERTEX
{

  D3DXVECTOR3 position; // The position
  D3DCOLOR    color;    // The color
  FLOAT       tu, tv;   // The texture coordinates
};

typedef struct tagGameDescription
{
	TCHAR szGameName[LEN_GAME_NAME];
	int  iGenre;
	int iPlatform;
	int iReleaseYear;
	int iPlayYear;

	tagGameDescription()
	{
		iGenre = -1;
		iPlatform = -1;
		iReleaseYear = -1;
		iPlayYear = -1;

		memset(szGameName, 0, sizeof(TCHAR) * LEN_GAME_NAME);

	}

}GameDescription;

class membuf : public std::wstreambuf // <- !!!HERE!!! 
{    
public:        

	membuf(wchar_t* p, size_t n) 
	{ // <- !!!HERE!!!         
		setg(p, p, p + n);     
	} 
}; 