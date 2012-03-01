
#pragma once

#include <d3dx9.h>

//Define a FVF for our terrain
#define TERRAIN_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

class CTerrain  
{
private:
	//Define a custom vertex for our terrain
	struct TERRAIN_CUSTOMVERTEX
	{
		FLOAT x, y, z;		//Position of vertex in 3D space
		FLOAT nx, ny, nz;	//Lighting Normal
		FLOAT tu, tv;		//Texture coordinates
	};

public:
	bool SetMaterial(D3DCOLORVALUE rgbaDiffuse, D3DCOLORVALUE rgbaAmbient, D3DCOLORVALUE rgbaSpecular, D3DCOLORVALUE rgbaEmissive, float rPower);
	bool SetTexture(const TCHAR* szTextureFilePath);
	bool SetSize(WORD wRows, WORD wCols, float rTileSize, WORD wMaxHeight);
	DWORD Render();
	CTerrain(LPDIRECT3DDEVICE9 pD3DDevice, WORD wRows = 10, WORD wCols = 10, float rTileSize = 10.0, WORD wMaxHeight = 15);
	virtual ~CTerrain();

private:
	bool CreateIndexBuffer();
	D3DXVECTOR3 GetTriangeNormal(D3DXVECTOR3* vVertex1, D3DXVECTOR3* vVertex2, D3DXVECTOR3* vVertex3);
	bool UpdateVertices();
	HRESULT CreateVertexBuffer();
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DTEXTURE9 m_pTexture2;
	D3DMATERIAL9 m_matMaterial;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;	

	WORD m_wRows;
	WORD m_wCols;
	WORD m_wMaxHeight;
	float m_rTileSize;

	DWORD m_dwNumOfVertices;	
	DWORD m_dwNumOfIndices;
	DWORD m_dwNumOfPolygons;

};