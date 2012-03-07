#pragma once

class CGroundPlane
{
public:

  CGroundPlane();
  ~CGroundPlane();

  HRESULT RestoreDeviceObjects(
    LPDIRECT3DDEVICE9 pDev, const TCHAR *strTextureFilename,
    float fWidth, float fHeight, int iNumGridSquares);
  void InvalidateDeviceObjects();
  void DeleteDeviceObjects();

  LPDIRECT3DTEXTURE9 GetTexture() { return(m_Texture); }
  LPDIRECT3DVERTEXBUFFER9 GetVB() { return(m_pvbGround); }
  LPDIRECT3DINDEXBUFFER9 GetIB()  { return(m_pibGround); }
  int GetNumVerts() { return(m_iNumVerts); }
  int GetNumIndices() { return(m_iNumIndices); }
  D3DXPLANE GetPlane() { return(m_Plane); }

private:
  LPDIRECT3DDEVICE9 m_pd3dDevice;
  LPDIRECT3DTEXTURE9 m_Texture;
  LPDIRECT3DVERTEXBUFFER9 m_pvbGround;
  LPDIRECT3DINDEXBUFFER9 m_pibGround;
  int m_iNumVerts;
  int m_iNumIndices;
  D3DXPLANE m_Plane;

};