#pragma once

#include <windows.h>
#include "video.h"
#include "font.h"

class Sample
{
public :
	Sample();
	~Sample();

	bool Init(IDirect3DDevice9* pd3dDevice);
	void Paint(HWND hwnd);

	void Process(float delta);
	void Update(float delta);
	void Render();

private :
	Video * m_video;
	FontManager * m_font;
	float m_time;
} ;

