#include "stdafx.h"

#include "D3DLine.h"

//-----------------------------------------------------------------------------
const unsigned long CLine2D::Vtx::fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

//-----------------------------------------------------------------------------
// NAME : CLine2D::CLine2D()
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 4:10
//-----------------------------------------------------------------------------
CLine2D::CLine2D()
{
	m_pDevice = 0L;
	m_VtxBuff = 0L;
	m_MaxLines = 0;
}
//-----------------------------------------------------------------------------
// NAME : CLine2D::~CLine2D()
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 4:10
//-----------------------------------------------------------------------------
CLine2D::~CLine2D()
{
	if (m_VtxBuff)
	{
		CLine2D::Clear();
	}
}
//-----------------------------------------------------------------------------
// NAME : CLine2D::Create()
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 4:11
//-----------------------------------------------------------------------------
bool CLine2D::Create(ID3DDevice* device, int maxLines)
{
	_ASSERTE(device);
	_ASSERTE(maxLines > 0);

	m_Lines.reserve(maxLines+1);
	m_MaxLines = maxLines;
	return Load_D3DObjects(device);
}
//-----------------------------------------------------------------------------
// NAME : CLine2D::Clear()
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 4:14
//-----------------------------------------------------------------------------
void CLine2D::Clear()
{
	if (m_VtxBuff)
	{
		m_VtxBuff->Release();
		m_VtxBuff = 0L;

		m_pDevice = 0L;
		m_Lines.clear();
		m_Offsets.clear();
		m_MaxLines = 0;
	}
}
//-----------------------------------------------------------------------------
// NAME : CLine2D::Load_D3DObjects()
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 4:16
//-----------------------------------------------------------------------------
bool CLine2D::Load_D3DObjects(ID3DDevice* device)
{
	_ASSERTE(device);
	HRESULT hr;

	hr = device->CreateVertexBuffer(
					m_MaxLines * sizeof(Vtx) + 1,	// vtx cnt = max letters * 6
					D3DUSAGE_WRITEONLY,				// write only mode
					Vtx::fvf,						// vertex format
					D3DPOOL_MANAGED,				// use default pool
					&m_VtxBuff, NULL);
	if (FAILED(hr))
	{
		_RPT0(_CRT_WARN, "CLine2D::Load_D3DObjects() failed.\n");
		return false;
	}

	m_pDevice = device;
	return true;
}
//-----------------------------------------------------------------------------
// NAME : CLine2D::Unload_D3DObjects()
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 4:24
//-----------------------------------------------------------------------------
void CLine2D::Unload_D3DObjects()
{
	if (m_VtxBuff)
	{
		m_VtxBuff->Release();
		m_VtxBuff = 0L;
	}
}
//-----------------------------------------------------------------------------
// NAME : CLine2D::Push()
// PRAM : None
// DESC : 두점사이의 선을 넣는다.
// LAST : 2003.7.5, 5:19
//-----------------------------------------------------------------------------
void CLine2D::Push(int x1, int y1, int x2, int y2, unsigned long color)
{
	Cut();
	Push(x1, y1, color);
	Push(x2, y2, color);
}
//-----------------------------------------------------------------------------
// NAME : CLine2D::Push()
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 5:19
//-----------------------------------------------------------------------------
void CLine2D::Push(int x, int y, unsigned long color)
{
	m_Lines.push_back(Vtx(x, y, color));
	++m_Offsets.back();
}
//-----------------------------------------------------------------------------
// NAME : CLine2D::Cut()
// PRAM : None
// DESC : 스트립 방식의 선을 끊는다.
// LAST : 2003.7.5, 9:14
//-----------------------------------------------------------------------------
void CLine2D::Cut()
{
	m_Offsets.push_back(0);
}
//-----------------------------------------------------------------------------
// NAME : CLine2D::Draw()
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 4:25
//-----------------------------------------------------------------------------
void CLine2D::Draw()
{
	m_pDevice->SetFVF(Vtx::fvf);
	m_pDevice->SetTexture(0, 0L);
	m_pDevice->SetStreamSource(0, m_VtxBuff, 0, sizeof(Vtx));
	
	int start = 0;
	OffsetVect::const_iterator end = m_Offsets.end();
	for(OffsetVect::const_iterator it=m_Offsets.begin() ;
	it != end ; ++it)
	{
		m_pDevice->DrawPrimitive(D3DPT_LINESTRIP, start, (*it)-1);
		start += *it;
	}

	m_Lines.clear();
	m_Offsets.clear();
}
//-----------------------------------------------------------------------------
// NAME : 
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 9:40
//-----------------------------------------------------------------------------
void CLine2D::Begin()
{
	m_Lines.clear();
	m_Offsets.resize(1);
	m_Offsets.front() = 0;
}
//-----------------------------------------------------------------------------
// NAME : 
// PRAM : None
// DESC : None
// LAST : 2003.7.5, 9:40
//-----------------------------------------------------------------------------
void CLine2D::End()
{
	if (m_Lines.empty())
		return;

	Vtx* v;
	m_VtxBuff->Lock(0, m_Lines.size()*sizeof(Vtx), (void**)&v, NULL);
	
	VtxVect::iterator iter = m_Lines.begin();
	Vtx* pLineBuffer = (Vtx*)iter;

	memcpy(v, pLineBuffer, m_Lines.size()*sizeof(Vtx));
	m_VtxBuff->Unlock();
}