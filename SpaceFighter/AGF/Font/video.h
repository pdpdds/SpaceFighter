#pragma once

struct	Vertex {
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
} ;

#define _POLYFAN				0x0000
//#define _POLYTRISTRIP			0x0001
//#define _POLYTRILIST			0x0002

//#define _POLYWIRE				0x0004

#define _COORDMASK				0x0010
#define _COORDWORLD				0x0000
#define _COORDPROJECTED			0x0010

#define _TEXBLENDMASK			0x0700
#define _TEXBLENDNORMAL			0x0000
#define _TEXBLENDADD			0x0100
#define _TEXBLENDMODULATE		0x0200
#define _TEXBLENDINVMODULATE	0x0300

//#define _NOCOLOR				0x1000
#define _NOCULL					0x2000

class	Video
{
public :
	virtual ~Video() {}

	virtual int CreateTexture(int w, int h) = 0;
	virtual void ReleaseTexture(int texidx) = 0;
	virtual void UpdateTexture(int texidx, unsigned char *ptr, int x, int y, int w, int h, int bpl, unsigned char (* palette)[3] = 0l) = 0;

	virtual void SetViewport(float fov, float nearplane, float farplane, float aspect=1.0f) {}

	virtual void Clear(unsigned long color=0x000000, float z=1.0f) = 0;

	virtual void SetTexture(int tex) {}
	virtual void DrawPolygon(Vertex *v, int vtxn, unsigned long type=0) {}

	virtual void BeginScene() {}
	virtual void EndScene() {}

	virtual void Paint() {}
} ;
