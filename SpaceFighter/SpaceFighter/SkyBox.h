#pragma once
#include <D3DX9.h>
//#include "D3DHelperFuncs.h"
//#include "CommonFuncs.h"
//#include "Camera.h"

class CSkyBox
{
public:

  static const int NUMFACES;

  CSkyBox();
  ~CSkyBox();

  HRESULT RestoreDeviceObjects(
    LPDIRECT3DDEVICE9 pDev, 
    const TCHAR *strTopTextureFilename,
    const TCHAR *strBottomTextureFilename,
    const TCHAR *strFrontTextureFilename,
    const TCHAR *strBackTextureFilename,
    const TCHAR *strLeftTextureFilename,
    const TCHAR *strRightTextureFilename);

  void InvalidateDeviceObjects();
  void DeleteDeviceObjects();

  enum BoxFace {
    Top = 0,
    Bottom,
    Left,
    Right,
    Front,
    Back
  };

  LPDIRECT3DTEXTURE9 GetTexture(BoxFace face) { return(m_Texture[face]); }
  
  void Render(D3DXMATRIX &matView);

	float GetSize(void) const { return(m_fSize); }
	void SetSize(const float data) { m_fSize = data; }
	  
private:
	float m_fSize;
  LPDIRECT3DDEVICE9 m_pd3dDevice;
  LPDIRECT3DTEXTURE9 m_Texture[6];
};