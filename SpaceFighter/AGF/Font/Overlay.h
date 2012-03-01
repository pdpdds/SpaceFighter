#pragma once

#include "video.h"

class	Overlay
{
public :
	Overlay(Video * video);

protected :
	void Render(int texhandle, float x, float y, float w, float h, float tu, float tv, float tw, float th, unsigned long type=0, unsigned long color=0xffffffff);

	Video * GetVideo() { return m_video; }

private:
	Video * m_video;
} ;
