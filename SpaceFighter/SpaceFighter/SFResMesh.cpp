#include "StdAfx.h"
#include "SFResMesh.h"
#include "SFSystem.h"
#include "d3dfile.h"
#include "SFCamera.h"
#include "OBB.h"

SFResMesh::SFResMesh(CD3DMesh* pMesh)
: m_pMesh(pMesh)
{
}

SFResMesh::~SFResMesh(void)
{
}

BOOL SFResMesh::Render( float fElapsedTime )
{
	IDirect3DDevice9* pD3DDevice = SFSystem::GetInstance()->GetD3DDevice();	

	m_pMesh->Render( pD3DDevice );   	

	return FALSE;
}

BOOL SFResMesh::Init()
{
	BYTE** pPtr = NULL;
	m_pMesh->GetLocalMesh()->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pPtr);

	if (FAILED(D3DXComputeBoundingSphere((D3DXVECTOR3*)pPtr, 
		m_pMesh->GetLocalMesh()->GetNumVertices(), 
		D3DXGetFVFVertexSize(m_pMesh->GetLocalMesh()->GetFVF()), &m_vCenter, &m_fRadius)))
	{
		return FALSE;
	}

	if (FAILED(D3DXComputeBoundingBox((D3DXVECTOR3*)pPtr, 
		m_pMesh->GetLocalMesh()->GetNumVertices(), 
		D3DXGetFVFVertexSize(m_pMesh->GetLocalMesh()->GetFVF()), &m_vMin, &m_vMax)))
	{
		return FALSE;
	}

	m_pMesh->GetLocalMesh()->UnlockVertexBuffer();

	if (FAILED(D3DXCreateSphere(SFSystem::GetInstance()->GetD3DDevice(), m_fRadius, 16, 16, &m_Sphere, NULL)))
	{
		return FALSE;
	}
	
	return TRUE;
}