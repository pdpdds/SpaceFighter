#include "stdafx.h"
#include "overlay.h"

	Overlay :: Overlay(Video * video)
{
	m_video = video;
}

#define MAGIC	-0.5f

void Overlay :: Render(int texhandle, float x, float y, float w, float h, float tu, float tv, float tw, float th, unsigned long type, unsigned long color)
{
	Vertex v[4];

	v[0].x = x + MAGIC;
	v[0].y = y + MAGIC;
	v[0].z = 0.0f;
	v[0].rhw = 1.0f;
	v[0].tu = tu;
	v[0].tv = tv;
	v[0].color = color;

	v[1].x = x + w + MAGIC;
	v[1].y = y + MAGIC;
	v[1].z = 0.0f;
	v[1].rhw = 1.0f;
	v[1].tu = tu + tw;
	v[1].tv = tv;
	v[1].color = color;

	v[2].x = x + w + MAGIC;
	v[2].y = y + h + MAGIC;
	v[2].z = 0.0f;
	v[2].rhw = 1.0f;
	v[2].tu = tu + tw;
	v[2].tv = tv + th;
	v[2].color = color;

	v[3].x = x + MAGIC;
	v[3].y = y + h + MAGIC;
	v[3].z = 0.0f;
	v[3].rhw = 1.0f;
	v[3].tu = tu;
	v[3].tv = tv + th;
	v[3].color = color;

	m_video->SetTexture(texhandle);
	m_video->DrawPolygon(v, 4, type|_NOCULL|_COORDPROJECTED);
}