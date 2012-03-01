#pragma once
#include <windows.h>
#include <d3d9.h>

#include "video.h"

class	Direct3D9 : public Video
{
public :
	Direct3D9();
	~Direct3D9();

	TCHAR * Init(IDirect3DDevice9* pd3dDevice, int w, int h, bool winmode);

	int CreateTexture(int w, int h);
	void ReleaseTexture(int texidx);
	void UpdateTexture(int texidx, unsigned char *ptr, int x, int y, int w, int h, int bpl, unsigned char (* palette)[3]);

	void SetViewport(float fov, float aspect, float zn, float zf);

	void Clear(unsigned long color, float z);
	void BeginScene();
	void EndScene();
	void Paint();

	void SetTexture(int idx);
	void DrawPolygon(Vertex *v, int n, unsigned long type);

protected :
	unsigned long Packed(int texidx, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	void * Lock(int texidx, int * pitch);
	void Unlock(int texidx);

private :
	D3DPRESENT_PARAMETERS m_d3dpp;
	IDirect3DDevice9 * m_device;
	unsigned long m_devid;
	IDirect3D9 * m_d3d9;
	D3DCAPS9 m_d3dcaps;
	HWND m_hwnd;

	int m_width, m_height;

	struct Texture {
		IDirect3DTexture9 * tex;
		D3DFORMAT fmt;
		int fmtidx;
		int w, h;
	}	m_texlist[128];

	void SetPolyType(unsigned long type);
	unsigned long m_oldtype;
	int m_oldtexhandle;

	void SetDefaultRenderState();
	D3DFORMAT FindDepthBufferFormat(D3DFORMAT suffmt);
} ;