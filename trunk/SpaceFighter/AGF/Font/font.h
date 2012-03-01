#pragma once
//	��Ʈ �̹������� �Ŵ�¡�Ѵ�
//

class	Font {
public :
	Font();
	~Font();

	bool Load(const char *fname);

	int CalcWidth(const char * text) const;

	int GetWidth(unsigned short code) const;
	int GetHeight() const { return m_maxheight; }

	int Draw(unsigned char * ptr, int pitch, unsigned short code);

private :
	struct CHINFO {
		int offset;
		unsigned char w, h;
	} * m_chinfo ;

	int m_maxheight;
	unsigned short m_cnt;
	unsigned short * m_codes;
	unsigned char * m_image;

	int SearchIndex(unsigned short) const;
} ;

#include "video.h"
#include "overlay.h"

//	��Ʈ�� �ؽ�ó�� �׸���
//

class	FontWorkspace : public Overlay {
public :
	FontWorkspace(Video * m_video);
	~FontWorkspace();

	void DrawPrimitive(float x, float y, float scale, unsigned long type, unsigned long color, void * p);

	void * GetTexPiece(const char *text, Font * font, int *width); // ��Ʈ�� �Ҵ�� ������ ��´�
	int GetWidth(void *p);

	void Flush();

private :
	struct	_TEXTURE {
		_TEXTURE * next;
		int handle;
	}	* m_curtex, * m_pivottex;

	struct	_TEXTPIECE {
		_TEXTPIECE * prev, * next;

		Font * font;		// ��Ʈ

		int updatecnt;		// ���� �����ӿ� ��µ� ���� �ִ� �� üũ

		_TEXTURE * tex;		// �ؽ�ó �ڵ�
		int u, v;			// �ؽ�ó ������ġ
		int w, h, fh;		// �ؽ�ó�� ����, ���� ��� (ù������ ���α���)

		int chw[3][2];		// 3���� ������ ������ ���� �����ؼ� �������� �� ������ �ʴ´�
		char text[1];		// �ؽ�Ʈ ����
	}	* m_piece;

	int m_updatecnt;

	unsigned char * m_image;

	int m_u, m_v, m_fheight;
	int m_vbottom, m_vtop;

	_TEXTPIECE * RemoveLink(_TEXTPIECE *head, _TEXTPIECE *p) {
		if (p->next)
			p->next->prev = p->prev;
		if (p->prev)
			p->prev->next = p->next;
		else
			return p->next;
		return head;
	}

	_TEXTPIECE * AddLink(_TEXTPIECE *head, _TEXTPIECE *p) {
		if (head)
			head->prev = p;
		p->prev = 0l;
		p->next = head;
		return p;
	}

	_TEXTPIECE * RemovePage(_TEXTURE * tex);

	void WriteTexture(_TEXTPIECE * p);
	void UpdateTexture(_TEXTURE * tex, int top, int bottom);
} ;

class	FontManager {
public :
	FontManager(Video * video) ;
	~FontManager() ;

	bool SetFont(const char *fname);
	void SetColor(unsigned long color) { m_color = color; }
	void SetPolyType(unsigned long type) { m_polytype = type; }

	int Print(float x, float y, const char *str, float scale=1.0f);

	void Flush();

	void Reset();

private :
	FontWorkspace * m_workspace;
	Video * m_video;

	struct _FONTSET {
		_FONTSET * next;
		Font * font;
		char fname[1];
	}	* m_fontlist;

	Font * m_font;

	unsigned long m_color;
	unsigned long m_polytype;

	struct	PiecePoly {
		void * p;
		float x, y, scale;
		unsigned long color;
		unsigned long type;
	}	m_polylist[128];
	int m_polylistn;
} ;

