#include "stdafx.h"
#include "direct3d9.h"
#include "sample.h"

#define FIXED_FPS	30
#define SKIPTIME 1.0f

Sample :: Sample()
{
	m_time = 0.0f;
	m_font = NULL;
	m_video = NULL;
}

Sample :: ~Sample()
{
	if (m_font)
		delete m_font;
	if (m_video)
		delete m_video;
}

int m_tex;

#define _D3D_

bool Sample :: Init(IDirect3DDevice9* pd3dDevice)
{
	
	TCHAR * err;

	Direct3D9 * d3d = new Direct3D9;

	err = d3d->Init(pd3dDevice, 640, 480, true);

	if (err) {
		MessageBox(NULL, err, _T("D3D9 Error"), MB_OK);
		return false;
	}

	m_video = d3d;

	m_font = new FontManager(m_video);
	m_font->SetFont("����12.fnt");		// �̸� �ε�����
	m_font->SetFont("HY�߰��20.fnt");	// �̸� �ε�����

	m_video->SetViewport(3.14159f * 90.0f / 180.0f, 10.0f, 2000.0f);

	return true;
}

void Sample :: Paint(HWND hwnd)
{
	m_video->Paint();
}

void Sample :: Process(float delta)
{
	if (m_time > SKIPTIME)
		m_time = SKIPTIME;

#ifdef FIXED_FPS
	while(m_time >= (1.0f/FIXED_FPS))
	{
		Update((1.0f/FIXED_FPS));
		m_time -= (1.0f/FIXED_FPS);
	}
#else
	Update(m_time);
#endif

	Render();
}

void Sample :: Update(float delta)
{
}

void Sample :: Render()
{
	//m_video->Clear(0x40805a);
	//m_video->BeginScene();

	//
	m_video->SetTexture(NULL);

	/*m_font->SetColor(0xffffffff);
	m_font->SetPolyType(_TEXBLENDADD);

	m_font->SetFont("����12.fnt");
	m_font->Print(10.0f, 25.0f, "�������� ���ߴ� �������� ������");
	m_font->Print(10.0f, 50.0f, "�� �ȿ� ���� �� ������ �ʴ� �˰� ������ �d");

	m_font->SetFont("HY�߰��20.fnt");
	m_font->Print(10.0f, 225.0f, "�������� ���ߴ� �������� ������", 0.7f);
	m_font->Print(10.0f, 250.0f, "�� �ȿ� ���� �� ������ �ʴ� �˰� ������", 0.7f);
*/
	m_font->SetColor(0xff000000 ^ 0x00ffffff);
	m_font->SetPolyType(_TEXBLENDADD);

	/*m_font->SetFont("����12.fnt");
	m_font->Print(10.0f, 75.0f, "�������� ������ �Բ� �ִ� �ʿ���");
	m_font->Print(10.0f, 100.0f, "�� �ȿ� ���� �� ������ ������ �� ������");*/

	m_font->SetFont("HY�߰��20.fnt");
	m_font->Print(10.0f, 275.0f, "�������� ������ �Բ� �ִ� �ʿ���", 1.2f);
	m_font->Print(10.0f, 300.0f, "�� �ȿ� ���� �� ������ ������ �� ������");

	//	words from �����ټ�������

	m_font->Flush();

	//

	//m_video->EndScene();
	//m_video->Paint();
}