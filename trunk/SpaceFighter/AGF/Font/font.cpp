#include "stdafx.h"
#include "font.h"
#include <stdio.h>
#include <string.h>
#ifdef _DEBUG
#include <crtdbg.h> 
#endif

//	-- Font -----------------------------------------------------
//

#define CHAR_GAP	0 // 글자간 여백

	Font :: Font()
{
	m_codes = 0;
	m_chinfo = 0;
	m_image = 0;
	m_maxheight = 0;
}

	Font :: ~Font()
{
	if (m_codes)
		delete [] m_codes;
	if (m_chinfo)
		delete [] m_chinfo;
	if (m_image)
		delete [] m_image;
}

//	폰트를 읽어들인다
//

bool Font :: Load(const char *fname)
{
	FILE * fp;
	int i, size, len, offset;

	fp = fopen(fname, "rb");

	if (fp) {
		fread(&m_cnt, 1, 2, fp);
		fread(&size, 1, 4, fp);

		m_codes = new unsigned short [m_cnt];
		m_chinfo = new CHINFO [m_cnt];
		m_image = new unsigned char [size];

		for(i=0, offset=0; i<m_cnt; i++)
		{
			fread(&m_codes[i], 1, 2, fp);
			fread(&m_chinfo[i].w, 1, 1, fp);
			fread(&m_chinfo[i].h, 1, 1, fp);
			m_chinfo[i].offset = offset;

			if (m_chinfo[i].h > m_maxheight)
				m_maxheight = m_chinfo[i].h;

			len = (m_chinfo[i].w * m_chinfo[i].h + 3) / 4;

			fread(&m_image[offset], 1, len, fp);
			offset += len;
		}
		fclose(fp);

		return true;
	}

	return false;
}

//	글자의 인덱스를 찾는다
//

int Font :: SearchIndex(unsigned short code) const
{
	int i1=0, i2=m_cnt-1, c;

	do {
		c = (i1 + i2) >> 1;

		if (code > m_codes[c])
			i1 = c+1;
		else
			i2 = c;
	}	while(i1 < i2);
#ifdef _DEBUG
	if (m_codes[i1] != code)
		_ASSERT(!"폰트셋에문자없어요");
#endif
	return i1;
}

//	글자의 가로 길이를 구한다
//

int Font :: GetWidth(unsigned short code) const
{
	return m_chinfo[SearchIndex(code)].w + CHAR_GAP;
}

//	문장의 가로 길이를 계산해본다
//

int Font :: CalcWidth(const char * text) const
{
	int i, code, w;

	for(i=0, w=0; text[i] != '\0';) {
		code = (unsigned char)text[i++];

		if (code&0x80)
			code = (code<<8) | (unsigned char)text[i++];

		w += GetWidth(code);
	}
	return w;
}

//	메모리에 그린다
//

int Font :: Draw(unsigned char * ptr, int pitch, unsigned short code)
{
	CHINFO * info = &m_chinfo[SearchIndex(code)];
	unsigned char * img, byte;
	int x, y, i;

	img = &m_image[info->offset];

	for(y=0, i=8; y<info->h; y++, ptr+=pitch) {
		for(x=0; x<info->w; x++, i+=2, byte>>=2) {
			if (i >= 8) {
				byte = *img++;
				i = 0;
			}
			ptr[x] = byte & 0x03;
		}
#if CHAR_GAP > 0
		for(; x<info->w+CHAR_GAP; x++)
			ptr[x] = 0;
#endif
	}

	return info->w + CHAR_GAP;
}

//	-- FontWorkspace --------------------------------------------
//

#define TEX_W	128
#define TEX_H	128

	FontWorkspace :: FontWorkspace(Video * video) : Overlay(video)
{
	m_curtex = m_pivottex = new _TEXTURE;
	m_curtex->handle = 0;
	m_curtex->next = m_curtex;

	m_image = new unsigned char [TEX_W * TEX_H];
	memset(m_image, 0, TEX_W * TEX_H);

	m_piece = NULL;

	m_u = m_v = 0;
	m_vtop = m_vbottom = 0;
	m_fheight = 0;

	m_updatecnt = 1;
}

//	텍스처 해제하고, 등록된 문장들 제거
//

	FontWorkspace :: ~FontWorkspace()
{
	_TEXTURE * t = m_curtex, * pt;
	do {
		pt = t;
		t = t->next;
		if (pt->handle)
			GetVideo()->ReleaseTexture(pt->handle);
		delete pt;
	}	while(t != m_curtex);

	_TEXTPIECE * next, * p;
	for(p=m_piece; p; p=next) {
		next = p->next;
		delete p;
	}

	delete [] m_image;
}

//	텍스처 올려야 할 거 있으면 올리고 나머지 변수들 초기화 하자
//

#define _MONITOR

void FontWorkspace :: Flush()
{
	UpdateTexture(m_curtex, m_vtop, m_vbottom);

	m_vtop = m_vbottom = m_v;
	m_pivottex = m_curtex;
	m_fheight = 0;
	m_updatecnt++;

#ifdef _MONITOR
	_TEXTURE * t=m_curtex;
	int i=0;
	do {
		Render(t->handle, 80.0f + (64.0f+5.0f) * i++, 400.0f-64.0f, 64, 64, 0, 0, 1, 1);
		t = t->next;
	}	while(t != m_curtex);
#endif
}

int FontWorkspace :: GetWidth(void *p)
{
	return ((_TEXTPIECE *)p)->w;
}

void * FontWorkspace :: GetTexPiece(const char *text, Font * font, int *width)
{
	_TEXTPIECE * p;

//	1.	이미 올려진 것 중에 폰트 같고, 내용도 같은 것이 있나 ?
//
	for(p=m_piece; p; p=p->next)
		if (p->font == font && !strcmp(p->text, text))
			break;

//	2.	없으면 올리고 리스트에 추가한다
//
	if (p == NULL) {
		p = (_TEXTPIECE*) (new char [sizeof(_TEXTPIECE) + strlen(text)]);
		strcpy_s(p->text, text);
		p->font = font;
		p->updatecnt = 0;

		m_piece = AddLink(m_piece, p);
		WriteTexture(p);
	}

//	3.	이번 프레임에 렌더링함 표시
//
	p->updatecnt = m_updatecnt;

	if (width)
		*width = p->w;

	return (void*) p;
}

//	텍스처에 적힌 내용중에서 해당 텍스처 페이지에 할당된 것들을 제거한다
//	단, 현 프레임에 렌더링 되어야 하는 녀석들은 m_reserve 리스트에 넣어둔다
//
FontWorkspace::_TEXTPIECE * FontWorkspace :: RemovePage(_TEXTURE * tex)
{
	_TEXTPIECE * p, * next;
	_TEXTPIECE * list = NULL;

	for(p=m_piece; p; ) {
		next = p->next;

		if (p->tex == tex) {
			m_piece = RemoveLink(m_piece, p);

			if (p->updatecnt == m_updatecnt) {
				list = AddLink(list, p);
			}	else {
				delete p;
			}
		}

		p = next;
	}

	return list;
}

//	텍스처에 글자 이미지를 그린다.
//
void FontWorkspace :: WriteTexture(_TEXTPIECE * p)
{
	int i, code, w, chpos=0, line=0;
	Font * font = p->font;
	_TEXTPIECE * rest = NULL;

	if (m_fheight < font->GetHeight())
		m_fheight = font->GetHeight();

//	1.	텍스처의 첫번째 위치를 기록해둔다
//
	p->u = m_u;
	p->v = m_v;
	p->h = font->GetHeight();
	p->fh = m_fheight;
	p->w = 0;
	p->tex = m_curtex;
	p->updatecnt = m_updatecnt;

	for(i=0, chpos=0; p->text[i] != '\0';) {
		chpos = i;

//		2.	문자열의 한 글자를 읽는다.
//
		code = (unsigned char)p->text[i++];

		if (code&0x80)
			code = (code<<8) | (unsigned char)p->text[i++];

//		3.	글자의 가로 길이를 얻는다.
//
		w = font->GetWidth(code);

//		4.	텍스처 가로크기를 넘는 경우에는 다음줄로 넘겨서 계속 그리도록 한다.
//			시작점의 경우 폰트의 크기에 따라 다음줄로 넘겨야 할 때가 있다.
//
		if (m_u + w > TEX_W || m_fheight + m_v >= TEX_H) {
//			5. 다음 줄로 넘긴다
//
			m_u = 0;
			m_v += m_fheight + 1;
			m_fheight = font->GetHeight();

//			6. 다음 줄이 페이지를 넘어갈 경우 다음 페이지에 그린다
//
			if (m_v + font->GetHeight() >= TEX_H) {
//				7. 먼저 현재 페이지를 업로드한다.
//
				UpdateTexture(m_curtex, m_vtop, m_v);//TEX_H-1);

//				8.	다음장으로 넘어간다.
//					만약 다음 장이 첫장(프레임시작할때)이면 한장 더 할당해야 한다. (순환되게 구성되어 있다.)
//
				if (m_curtex->next == m_pivottex) {
					_TEXTURE * t = new _TEXTURE;
					t->handle = 0;
					t->next = m_curtex->next;
					m_curtex->next = t;
				}	else {
//					9.	새롭게 사용할 텍스처에 사용해야될 문장이 있으면 남기고, 아닌 것은 삭제한다.
//
					rest = RemovePage(m_curtex->next);
				}

				m_curtex = m_curtex->next;

				m_u = m_v = 0;
				m_vbottom = 0;
				m_vtop = 0;

				memset(m_image, 0, TEX_W*TEX_H);
			}

//			10.	시작부터 넘겨야 하는 경우에는 그 곳을 시작점으로 한다.
//				그리고 특별히 긴 문장이 아닌 경우에 글자 구하는 부하를 줄이기 위해 정보를 저장한다.
//
			if (chpos == 0) {
				p->u = m_u;
				p->v = m_v;
				p->fh = m_fheight;
				p->tex = m_curtex;
			}	else
			if (line < sizeof(p->chw)/sizeof(*p->chw)) {
				p->chw[line][0] = chpos;
				p->chw[line][1] = p->w;
				line++;
			}
		}

//		11.	텍스처 위치에 글자를 그린다
//
		font->Draw(m_image + m_u + m_v * TEX_W, TEX_W, code);

		m_u += w;
		p->w += w;
	}

//	12.	바닥 위치를 갱신한다
//
	if (m_vbottom < m_v + font->GetHeight() + 1)
		m_vbottom = m_v + font->GetHeight() + 1;

//	13.	새롭게 페이지 할당하면서 밀린 내용 다시 갱신한다
//
	if (rest) {
		_TEXTPIECE * next, * p = rest;
		for(; p; p=next) {
			next = p->next;

			m_piece = AddLink(m_piece, p);
			WriteTexture(p);
		}
	}
}

void FontWorkspace :: DrawPrimitive(float x, float y, float scale, unsigned long type, unsigned long color, void * ptr)
{
	int u1, u2, v, remain;
	int line=0;
	_TEXTPIECE * p = (_TEXTPIECE *) ptr;
	_TEXTURE * tex;
	int i=0, chwide;

//	1.	텍스처와 시작 위치를 얻는다
//
	tex = p->tex;
	u1 = u2 = p->u;
	v = p->v;

	remain = p->w;

	do {
//		2.	현재 라인에서 문장이 차지하는 가로 라인 길이를 구한다.
//
		if (remain + u1 <= TEX_W) {
			u2 = u1 + remain;
		}	else {
			if (line < sizeof(p->chw)/sizeof(*p->chw)) {
				i = p->chw[line][0];
				u2 += line == 0 ? p->chw[line][1] : p->chw[line][1] - p->chw[line-1][1];
			}	else
			for(; p->text[i] != '\0'; ) {
				int code = (unsigned char)p->text[i++];

				if (code&0x80)
					code = (code<<8) | (unsigned char)p->text[i++];

				chwide = p->font->GetWidth(code);

				if (u2 + chwide > TEX_W)
					break;

				u2 += chwide;
			}
		}

//		3.	그린다.
//
		Render(tex->handle, x, y, (u2 - u1) * scale, p->h * scale,
			(float)u1 / TEX_W, (float)v / TEX_H, (float)(u2 - u1) / TEX_W, (float)p->h / TEX_H, type, color);//(line&1) == 0 ? color : (color^0x00ffff));

		remain -= u2 - u1;

		if (remain > 0) {
//			4.	아직 덜 그렸다. x 위치 증가.
//
			x += (u2 - u1) * scale;

//			5.	다음 라인 이미지로.
//				(chw 에서 얻는 것이 아닐 경우 넘기는 타이밍이 다음 첫글자까지 체크한 후다.)
//
			v += (line == 0 ? p->fh : p->h) + 1;
			u1 = 0;
			u2 = line < sizeof(p->chw)/sizeof(*p->chw) ? 0 : chwide;

//			6.	다음페이지로 넘겨야 할 경우 처리한다.
//
			if (v + p->h + 1 > TEX_H) {
				tex = tex->next;
				v = 0;
			}

			line++;
		}
	}	while(remain > 0);
}

//	텍스처 이미지를 올린다
//

void FontWorkspace :: UpdateTexture(_TEXTURE * tex, int top, int bottom)
{
	static unsigned char palette[4][3] = {
		0, 0, 0, 
		128, 128, 128,
		196, 196, 196,
		255, 255, 255,
	} ;

	if (top < bottom) {
		if (tex->handle == 0)
			tex->handle = GetVideo()->CreateTexture(TEX_W, TEX_H);

		GetVideo()->UpdateTexture(tex->handle, m_image + top * TEX_W,
			0, top, TEX_W, bottom - top + 1, TEX_W,
			palette);
	}
}

//	-- FontManager ----------------------------------------------
//

	FontManager :: FontManager(Video * video)
{
	m_workspace = new FontWorkspace(video);
	m_video = video;
	m_font = NULL;
	m_fontlist = NULL;

	m_color = 0xffffffff;
	m_polytype = 0;

	m_polylistn = 0;
}

	FontManager :: ~FontManager()
{
	_FONTSET * f, * next;

	delete m_workspace;

	for(f=m_fontlist; f; f=next) {
		next = f->next;
		delete f->font;
		delete f;
	}
}

void FontManager :: Reset()
{
	delete m_workspace;
	m_workspace = new FontWorkspace(m_video);
}

bool FontManager :: SetFont(const char *fname)
{
//	1.	이전에 읽었던 것 중에 있는지 체크한다.
//
	_FONTSET * f;

	for(f=m_fontlist; f; f=f->next) {
		if (!strcmp(f->fname, fname)) {
			m_font = f->font;
			return true;
		}
	}

//	2.	없다. 추가한다.
//
	m_font = new Font;
	if (m_font->Load(fname) == false) {
		delete m_font;
		m_font = NULL;
		return false;
	}

	f = (_FONTSET*) (new char [sizeof(_FONTSET) + strlen(fname)]);
	f->font = m_font;
	strcpy(f->fname, fname);
	f->next = m_fontlist;
	m_fontlist = f;
	return true;
}

void FontManager :: Flush()
{
	m_workspace->Flush();

	for(int i=0; i<m_polylistn; i++)
		m_workspace->DrawPrimitive(m_polylist[i].x, m_polylist[i].y, m_polylist[i].scale, m_polylist[i].type, m_polylist[i].color, m_polylist[i].p);

	m_polylistn = 0;
}

int FontManager :: Print(float x, float y, const char *text, float scale)
{
	void * p;
	int w;

	if (m_polylistn > sizeof(m_polylist)/sizeof(*m_polylist))
		Flush();
	
	m_polylist[m_polylistn].p = p = m_workspace->GetTexPiece(text, m_font, &w);
	m_polylist[m_polylistn].color = m_color;
	m_polylist[m_polylistn].x = x;
	m_polylist[m_polylistn].y = y;
	m_polylist[m_polylistn].scale = scale;
	m_polylist[m_polylistn].type = m_polytype;
	m_polylistn++;

	return w;
}

//	noerror