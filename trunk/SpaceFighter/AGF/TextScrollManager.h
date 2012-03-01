#pragma once
#include <map>
#include "FontObject.h"

class CTextScrollManager : public CDirectXEntity
{
	typedef std::map<int, std::wstring> mapText;

public:
	CTextScrollManager(void);
	virtual ~CTextScrollManager(void);

	BOOL Initialize(RECT& ScrollArea);
	BOOL AddText(TCHAR* szText);
	void SetEnable(BOOL bEnable){m_bScrolling = bEnable;}
	BOOL LoadTextScrollData(TCHAR* szGameName);
	BOOL LoadTextScrollDataFromMemory( TCHAR* szGameName );

	virtual BOOL OnRender(float fElapsedTime) override;
	virtual BOOL OnResetDevice() override;
	virtual BOOL OnFrameMove( double fTime, float fElapsedTime) override;
	virtual BOOL OnLostDevice() override;
	virtual BOOL OnDestroyDevice() override;

protected:

private:
	RECT m_ScrollArea;
	CFontObject m_Font;

	float m_fTextStartPosY;
	float m_fVerticalScrollVelocity;

	BOOL m_bScrolling;

	int m_iTextIndex;

	mapText m_mapText;
};
