#pragma once
#include "SFRes.h"

class CD3DMesh;
class OBB;

class SFResMesh : public SFRes
{
public:
	SFResMesh(CD3DMesh* pMesh);
	virtual ~SFResMesh(void);

	virtual BOOL Render(float fElapsedTime) override;
	virtual BOOL Init() override;

	float GetRadius()                 { return m_fRadius; };
	void  SetRadius( float fRadius )  { m_fRadius = fRadius; };

	D3DXVECTOR3& GetMin(){return m_vMin;}
	D3DXVECTOR3& GetMax(){return m_vMax;}
	D3DXVECTOR3& GetCenter(){return m_vCenter;}

protected:

private:
	CD3DMesh* m_pMesh;
	D3DXVECTOR3 m_vCenter;
	float m_fRadius;

	D3DXVECTOR3 m_vMin;
	D3DXVECTOR3 m_vMax;

	LPD3DXMESH m_Sphere;
};
