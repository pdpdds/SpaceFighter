#pragma once

#include "Timer.h"

class CAnimFrame
{
public:
  CAnimFrame() { m_pTexture = NULL; }
  virtual ~CAnimFrame() { SAFE_RELEASE(m_pTexture); }

  LPDIRECT3DTEXTURE9 m_pTexture;
  float m_fTime;
};

class CAnimSequence {
public:
  CAnimSequence(LPDIRECT3DDEVICE9 pDev);
  virtual ~CAnimSequence() { ClearAllFrames(); SAFE_RELEASE(m_pVBQuad); }
  
  bool AddFrame(const TCHAR *strFilename, float fTime);
  void DeleteFrame(int index);
  void ClearAllFrames();
  int GetCurFrame(CTimer &timer);
  LPDIRECT3DTEXTURE9 GetCurFrameTexture(CTimer &timer) {
    try {
      int iCurFrame = GetCurFrame(timer);
      return(m_vFrames[iCurFrame]->m_pTexture);
    } catch(...) { }
    return(NULL);
  }

  void Render(CTimer &timer, D3DXMATRIX mat);
  void RenderBillboard(CTimer &timer, D3DXMATRIX mat);
  void BillboardRotation(D3DXMATRIX mat,float x, float y, float z, float scale); // 카메라에 대해서

protected:
  LPDIRECT3DVERTEXBUFFER9 m_pVBQuad;
  LPDIRECT3DDEVICE9 m_pd3dDevice;
  std::vector<CAnimFrame *> m_vFrames;  

private:
	D3DXMATRIX m_matBillboard;
};