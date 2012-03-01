#pragma once

class CLine2D
{
public:
	//**** type defines
	struct Vtx
	{
		float x, y, z, rhw;
		unsigned long color;

		Vtx(int _x, int _y, unsigned long _color) : x(_x-0.5f), y(_y-0.5f), z(0), rhw(0.1f), color(_color) {}
		static const unsigned long fvf;
	};

	typedef IDirect3DDevice9 ID3DDevice;
	typedef IDirect3DVertexBuffer9 ID3DVtx;
	typedef IDirect3DTexture9	   ID3DTex;
	typedef std::vector<Vtx>	VtxVect;
	typedef std::vector<int>	OffsetVect;

	//**** constructor / desturctor
	CLine2D();
	~CLine2D();
	bool Create(ID3DDevice* device, int maxLines=100);
	void Clear();

	//**** main method
	void  Push(int x1, int y1, int x2, int y2, unsigned long color);	// line list
	void  Push(int x1, int y1, unsigned long color);					// strip line
	void  Cut();
	void  Begin();
	void  End();
	void  Draw();

	// mem management
	bool  Load_D3DObjects(ID3DDevice* device);
	void  Unload_D3DObjects();

protected:
	//**** properties
	ID3DDevice*	m_pDevice;
	ID3DVtx*	m_VtxBuff;
	VtxVect		m_Lines;
	OffsetVect	m_Offsets;
	int			m_MaxLines;
};
