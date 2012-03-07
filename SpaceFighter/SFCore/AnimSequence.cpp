#include "stdafx.h"
#include "AnimSequence.h"

CAnimSequence::CAnimSequence(LPDIRECT3DDEVICE9 pDev)
{
  m_pVBQuad = NULL;
  m_pd3dDevice = pDev;
  CreateQuad(&m_pVBQuad, D3DPOOL_DEFAULT, 1.0f, 
    D3DCOLOR_ARGB(255,255,255,255), pDev);
}

bool CAnimSequence::AddFrame(const TCHAR *strFilename, float fTime)
{
  if (!m_pd3dDevice) return(false); 
  if (fTime <= 0) return(false);

  CAnimFrame *newframe = new CAnimFrame();
  
  // create a texture for this frame
  if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, 
             strFilename, &newframe->m_pTexture))) {
    delete newframe;
    return(false);
  }
  
  // add to vector
  newframe->m_fTime = fTime;
  m_vFrames.push_back(newframe);
  return(true);
}

void CAnimSequence::DeleteFrame(int index)
{
  delete *(m_vFrames.begin() + index);
  m_vFrames.erase(m_vFrames.begin() + index);
}

void CAnimSequence::ClearAllFrames()
{
  for (std::vector<CAnimFrame *>::iterator i = m_vFrames.begin(); 
       i != m_vFrames.end(); ++i) {
    delete *i;
  }
  m_vFrames.clear();
}

void CAnimSequence::Render(CTimer &timer, D3DXMATRIX mat)
{
  if (!m_pd3dDevice) return; 
  if (timer.GetTime() < 0) return;

	int iCurFrame = GetCurFrame(timer);

  if (iCurFrame >=(int) m_vFrames.size()) return;

  CAnimFrame *f = m_vFrames[iCurFrame];

  m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat);
  m_pd3dDevice->SetTexture(0, f->m_pTexture);
  m_pd3dDevice->SetFVF(D3DFVF_XYZ_DIFFUSE_TEX1);
  m_pd3dDevice->SetStreamSource(0, m_pVBQuad, 0, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));
  m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

void CAnimSequence::BillboardRotation(D3DXMATRIX mat,float x, float y, float z, float scale)
{

	D3DXMATRIX matScale, matTranslate, matTranspose, matBillboardRot, matWorld;	
	D3DXMATRIX matRot; // user-specified rotation
	D3DXMatrixTranspose(&matTranspose, &mat);
	D3DXMatrixIdentity(&matBillboardRot);

	matBillboardRot._11 = matTranspose._11;
	matBillboardRot._12 = matTranspose._12;
	matBillboardRot._13 = matTranspose._13;
	matBillboardRot._21 = matTranspose._21;
	matBillboardRot._22 = matTranspose._22;
	matBillboardRot._23 = matTranspose._23;
	matBillboardRot._31 = matTranspose._31;
	matBillboardRot._32 = matTranspose._32;
	matBillboardRot._33 = matTranspose._33;

	D3DXMatrixTranslation(&matWorld, x, y, z);
	D3DXMatrixScaling(&matScale, scale, scale, 1);
	m_matBillboard = matScale*matBillboardRot*matWorld;
}

int CAnimSequence::GetCurFrame(CTimer &timer)
{
  int iCurFrame = 0; float fTimeCount = 0.0f;
  for (std::vector<CAnimFrame *>::iterator i = m_vFrames.begin(); i != m_vFrames.end(); ++i) {
    fTimeCount += (*i)->m_fTime;
    if (timer.GetTime() <= fTimeCount) break;
    iCurFrame++;
  }
	return(iCurFrame);
}

void CAnimSequence::RenderBillboard( CTimer &timer, D3DXMATRIX mat )
{
	if (!m_pd3dDevice) return; 
	if (timer.GetTime() < 0) return;

	int iCurFrame = GetCurFrame(timer);

	if (iCurFrame >= (int)m_vFrames.size()) 
	{	
		timer.SetTime(0);
		iCurFrame = 0;
	}

	CAnimFrame *f = m_vFrames[iCurFrame];

	m_matBillboard = mat * m_matBillboard;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matBillboard);
	m_pd3dDevice->SetTexture(0, f->m_pTexture);
	m_pd3dDevice->SetFVF(D3DFVF_XYZ_DIFFUSE_TEX1);
	m_pd3dDevice->SetStreamSource(0, m_pVBQuad, 0, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

}