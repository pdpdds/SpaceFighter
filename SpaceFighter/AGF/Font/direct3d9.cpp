#include "stdafx.h"
#include "direct3d9.h"
#include "../DirectXSystem.h"

Direct3D9 :: Direct3D9()
{
	m_d3d9 = NULL;
	m_devid = 0;

	m_oldtexhandle = 0;

	memset(&m_d3dcaps, 0, sizeof(m_d3dcaps));
	memset(&m_texlist, 0, sizeof(m_texlist));
}

Direct3D9 :: ~Direct3D9()
{
	if (m_device)
		m_device->Release();

	if (m_d3d9)
		m_d3d9->Release();
}

#include <D3dx9math.h>

TCHAR * Direct3D9 :: Init(IDirect3DDevice9* pd3dDevice, int w, int h, bool winmode)
{
	/*D3DDISPLAYMODE d3ddm;

	m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_d3d9)
		return L"D3D_ERR_D3D_CREATE_FAIL";
	
	m_hwnd = hwnd;
	m_width = w;
	m_height = h;

	m_d3d9->GetDeviceCaps(m_devid, D3DDEVTYPE_HAL, &m_d3dcaps);

	if (winmode) {
//		if (!(m_d3dcaps.Caps2&D3DCAPS2_CANRENDERWINDOWED))
	//		return "D3D_ERR_INIT_WINMODE_FAIL";

		if (m_d3d9->GetAdapterDisplayMode(m_devid, &d3ddm) != D3D_OK)
			return L"D3D_ERR_INIT_DISPLAY_FAIL";

		m_d3dpp.Windowed = TRUE;
		m_d3dpp.BackBufferWidth = w;
		m_d3dpp.BackBufferHeight = h;
		m_d3dpp.BackBufferFormat = d3ddm.Format;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;

		m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//	m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}	else {
		//
	}

	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_d3dpp.hDeviceWindow = hwnd;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = FindDepthBufferFormat(m_d3dpp.BackBufferFormat);
	m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if (m_d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_device) != D3D_OK)
		return L"D3D_ERR_CREATE_DEVICE_FAILE";*/

	m_width = CDirectXSystem::GetInstance()->GetWidth();
	m_height = CDirectXSystem::GetInstance()->GetHeight();
	m_device =pd3dDevice;


	SetDefaultRenderState();
	Clear(0x000000, 1.0f);

	return NULL;
}

D3DFORMAT Direct3D9 :: FindDepthBufferFormat(D3DFORMAT suffmt)
{
	static D3DFORMAT recom_fmt[] = {
		D3DFMT_D32, D3DFMT_D24S8, D3DFMT_D16, D3DFMT_D15S1
	};
	int i;

	for(i=0; i<sizeof(recom_fmt)/sizeof(*recom_fmt); i++)
		if (m_d3d9->CheckDepthStencilMatch(m_devid, D3DDEVTYPE_HAL, suffmt, suffmt, recom_fmt[i]) == D3D_OK)
			return recom_fmt[i];

	return D3DFMT_D16;
}

void Direct3D9 :: SetDefaultRenderState()
{
	static struct {
		unsigned long stage;
		D3DTEXTURESTAGESTATETYPE type;
		unsigned long data;
	}	basic_texture_state[] = {
		0, D3DTSS_TEXCOORDINDEX, 0,
		0, D3DTSS_COLORARG1, D3DTA_TEXTURE,
		0, D3DTSS_COLORARG2, D3DTA_DIFFUSE,
		0, D3DTSS_COLOROP, D3DTOP_MODULATE,

		0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE,
		0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE,
		0, D3DTSS_ALPHAOP, D3DTOP_MODULATE,
	} ;

	static struct {
		D3DRENDERSTATETYPE type;
		unsigned long data;
	}	basic_render_state[] = {
		D3DRS_ZENABLE, TRUE,
		D3DRS_LIGHTING, FALSE,
		D3DRS_ALPHABLENDENABLE, TRUE,
	} ;

	int i;

	for(i=0; i<sizeof(basic_texture_state)/sizeof(*basic_texture_state); i++)
		m_device->SetTextureStageState(basic_texture_state[i].stage, basic_texture_state[i].type, basic_texture_state[i].data);

	for(i=0; i<sizeof(basic_render_state)/sizeof(*basic_render_state); i++)
		m_device->SetRenderState(basic_render_state[i].type, basic_render_state[i].data);

	m_oldtype = ~0;
	SetPolyType(0);
}

void Direct3D9 :: SetViewport(float fov, float zn, float zf, float aspect)
{
	D3DMATRIX mat;
	float tanfov;

	memset(&mat, 0, sizeof(mat));

	aspect *= ((float)m_width / (float)m_height);
	tanfov = (float) sin(fov * 0.5f) / (float) cos(fov * 0.5f);

	mat._11 = 1.0f / tanfov / aspect;
	mat._22 = 1.0f / tanfov;
	mat._33 = zf / (zf - zn);
	mat._43 = zn * zf / (zn - zf);
	mat._34 = 1.0f;

	m_device->SetTransform(D3DTS_PROJECTION, &mat);
}

void Direct3D9 :: Clear(unsigned long Color, float Z)
{
	DWORD Flags = D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER;
	D3DRECT * pRects = NULL;

	m_device->Clear(0, pRects, Flags, (D3DCOLOR) Color, Z, 0);
}

void Direct3D9 :: BeginScene()
{
	m_device->BeginScene();
}

void Direct3D9 :: EndScene()
{
	m_device->EndScene();
}

static struct _pixfmt {
	D3DFORMAT fmt;
	int cpp; // char pre pixel
	int r_r_shift, r_l_shift;
	int g_r_shift, g_l_shift;
	int b_r_shift, b_l_shift;
	int a_r_shift, a_l_shift;

	unsigned long Pack(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		return ((a>>a_r_shift) << a_l_shift) | ((r>>r_r_shift) << r_l_shift) | \
			((g>>g_r_shift) << g_l_shift) | ((b>>b_r_shift) << b_l_shift);
	}
}	g_pixfmt[] =  {
	{ D3DFMT_R5G6B5, 2, 3, 11, 2, 5, 3, 0, 8, 0 },
	{ D3DFMT_R8G8B8, 3, 0, 16, 0, 8, 0, 0, 8, 0 },
	{ D3DFMT_A1R5G5B5, 2, 3, 10, 3, 5, 3, 0, 7, 15 },
	{ D3DFMT_X1R5G5B5, 2, 3, 10, 3, 5, 3, 0, 8, 16 },
	{ D3DFMT_R3G3B2, 1, 5, 5, 5, 2, 6, 0, 8, 0 },
	{ D3DFMT_A4R4G4B4, 2, 4, 8, 4, 4, 4, 0, 4, 12 },
	{ D3DFMT_A8R8G8B8, 4, 0, 16, 0, 8, 0, 0, 0, 24 },
	{ D3DFMT_A8R3G3B2, 2, 5, 5, 5, 2, 6, 0, 0, 8 },
	{ D3DFMT_X8R8G8B8, 4, 0, 16, 0, 8, 0, 0, 8, 24 }
} ;

int Direct3D9 :: CreateTexture(int w, int h)
{
	D3DFORMAT fmt = D3DFMT_R5G6B5;
	int i, fmtidx;

	for(i=1; i<sizeof(m_texlist)/sizeof(*m_texlist); i++)
	{
		if (m_texlist[i].tex == NULL)
		{
			if (!FAILED(m_device->CreateTexture(w, h, 0, 0, fmt, D3DPOOL_MANAGED, &m_texlist[i].tex, NULL)))
			{
				m_texlist[i].fmt = fmt;
				m_texlist[i].w = w;
				m_texlist[i].h = h;

				for(fmtidx=0; g_pixfmt[fmtidx].fmt != fmt; fmtidx++) ;
				m_texlist[i].fmtidx = fmtidx;
				return i;
			}
		}
	}

	return 0;
}

void Direct3D9 :: ReleaseTexture(int texidx)
{
	m_texlist[texidx].tex->Release();
	m_texlist[texidx].tex = NULL;
}

void * Direct3D9 :: Lock(int texidx, int * pitch)
{
	D3DLOCKED_RECT rect;

	if (m_texlist[texidx].tex->LockRect(0, &rect, NULL, 0) == D3D_OK)
	{
		if (pitch)
			*pitch = rect.Pitch;
		return rect.pBits;
	}
	return NULL;
}

void Direct3D9 :: Unlock(int texidx)
{
	m_texlist[texidx].tex->UnlockRect(0);
}

unsigned long Direct3D9 :: Packed(int texidx, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return g_pixfmt[m_texlist[texidx].fmtidx].Pack(r, g, b, a);
}

void Direct3D9 :: UpdateTexture(int texidx, unsigned char *ptr, int x, int y, int w, int h, int bpl, unsigned char (* palette)[3])
{
	unsigned long code;
	unsigned char * tag, * src, * p, * pbits;
	int i, j, fmtidx, pitch;
		
	pbits = (unsigned char*) Lock(texidx, &pitch);

	if (pbits) {
		fmtidx = m_texlist[texidx].fmtidx;

		pbits += x + y * pitch;

		for(i=0; i<h; i++, pbits+=pitch, src+=bpl) {
			src = ptr + i * bpl;
			tag = (unsigned char*) pbits;

			for(j=0; j<w; j++, tag+=g_pixfmt[fmtidx].cpp) {
				p = !palette ? &src[j*3] : palette[src[j]];

				code = g_pixfmt[fmtidx].Pack(p[2], p[1], p[0], 255);
				memcpy(tag, &code, g_pixfmt[fmtidx].cpp);
			}
		}

		Unlock(texidx);
	}
}

void Direct3D9 :: SetTexture(int idx)
{
	if (m_oldtexhandle != idx) {
		m_oldtexhandle = idx;
		m_device->SetTexture(0, idx != 0 ? m_texlist[idx].tex : NULL);
	}
}

void Direct3D9 :: SetPolyType(unsigned long type)
{
	int xor;
	if (type != m_oldtype) {
		xor = m_oldtype ^ type;
		m_oldtype = type;

		if (xor&_TEXBLENDMASK) {
			switch(type&_TEXBLENDMASK) {
				case _TEXBLENDNORMAL :
					m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					break;
				case _TEXBLENDADD :
					m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
					break;
				case _TEXBLENDMODULATE :
					m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
					m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
					break;
				case _TEXBLENDINVMODULATE :
					m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
					m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
					break;
			}
		}
		if (xor&_NOCULL)
			m_device->SetRenderState(D3DRS_CULLMODE, (type&_NOCULL) ? D3DCULL_NONE : D3DCULL_CCW);
//		if (xor&_POLYWIRE)
//			m_device->SetRenderState(D3DRS_FILLMODE , (type&_POLYWIRE) ? D3DFILL_WIREFRAME  : D3DFILL_SOLID);
	}
}

void Direct3D9 :: DrawPolygon(Vertex *v, int n, unsigned long type)
{
	SetPolyType(type);

	if (type&_COORDPROJECTED) {
		m_device->SetFVF(D3DFVF_XYZRHW|D3DFVF_TEX1|D3DFVF_DIFFUSE);
		m_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, n-2, v, sizeof(Vertex));
	}	else {
		// miss
	}
}

void Direct3D9 :: Paint()
{
	m_device->Present(NULL, NULL, m_hwnd, NULL);
}
