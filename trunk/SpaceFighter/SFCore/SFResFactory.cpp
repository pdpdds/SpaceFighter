#include "StdAfx.h"
#include "SFResFactory.h"
#include "SFSystem.h"
#include "dxfile.h"
#include "D3DFile.h"
#include "AnimSequence.h"
#include "SFResMesh.h"
#include "SFResAni.h"
#include "assert.h"

SFResFactory::SFResFactory(void)
{
}

SFResFactory::~SFResFactory(void)
{
}


void SFResFactory::CreateExplosion(CSprite &spr,
										float PosX, float PosY, float PosZ,
										float RadiusX, float RadiusY, float RadiusZ,
										float Size, float SizeDelta,
										float Time, float TimeDelta)
{

	spr.Pos() = D3DXVECTOR3(PosX, PosY, PosZ) + 
		RandomNumber(
		D3DXVECTOR3(-RadiusX, -RadiusY, -RadiusZ),
		D3DXVECTOR3(RadiusX, RadiusY, RadiusZ));

	spr.SetSize(Size + RandomNumber(-SizeDelta, SizeDelta));

	spr.Timer().BeginWithDelay(Time+RandomNumber(-TimeDelta, TimeDelta));
}

SFRes* SFResFactory::Create( unsigned int ResID )
{
	SFRes* pRes = NULL;	

	switch(ResID)
	{
	case PLANE_1:
		{
			CD3DMesh* pD3DMesh = new CD3DMesh;

			if( FAILED( pD3DMesh->Create(SFSystem::GetInstance()->GetD3DDevice(), L"./GameObject/plane_2.x") ) )
				return NULL;

			pD3DMesh->RestoreDeviceObjects( SFSystem::GetInstance()->GetD3DDevice() );
			pRes = new SFResMesh(pD3DMesh);
		}
		break;
	case HERO_PLANE:
		{
			CD3DMesh* pD3DMesh = new CD3DMesh;

			if( FAILED( pD3DMesh->Create(SFSystem::GetInstance()->GetD3DDevice(), L"./GameObject/hero.x") ) )
				return NULL;

			pD3DMesh->RestoreDeviceObjects( SFSystem::GetInstance()->GetD3DDevice() );
			pRes = new SFResMesh(pD3DMesh);
		}
		break;
	case STAR_SAIL:
		{
			CD3DMesh* pD3DMesh = new CD3DMesh;

			if( FAILED( pD3DMesh->Create(SFSystem::GetInstance()->GetD3DDevice(), L"./GameObject/star sail.x") ) )
				return NULL;

			pD3DMesh->RestoreDeviceObjects( SFSystem::GetInstance()->GetD3DDevice() );
			pRes = new SFResMesh(pD3DMesh);
		}
		break;
	case ITEM_BOX:
		{
			CD3DMesh* pD3DMesh = new CD3DMesh;

			if( FAILED( pD3DMesh->Create(SFSystem::GetInstance()->GetD3DDevice(), L"./GameObject/ItemBox.x") ) )
				return NULL;

			pD3DMesh->RestoreDeviceObjects( SFSystem::GetInstance()->GetD3DDevice() );
			pRes = new SFResMesh(pD3DMesh);
		}
		break;
	case EXPLSION_EFFECT_1:
		{
			CAnimSequence* pSeq = new CAnimSequence(SFSystem::GetInstance()->GetD3DDevice());
			
			if (false == pSeq->AddFrame(L"./media/exp0.tga", 0.03f) ||
			false == pSeq->AddFrame(L"./media/exp1.tga", 0.03f) ||
			false == pSeq->AddFrame(L"./media/exp2.tga", 0.03f) ||
			false == pSeq->AddFrame(L"./media/exp3.tga", 0.03f) ||
			false == pSeq->AddFrame(L"./media/exp4.tga", 0.03f) ||
			false == pSeq->AddFrame(L"./media/exp5.tga", 0.03f) ||
			false == pSeq->AddFrame(L"./media/exp6.tga", 0.03f) ||
			false == pSeq->AddFrame(L"./media/exp7.tga", 0.03f))
			{
				assert(0);
			}
			else
			{
				CSprite* pSprite = new CSprite(pSeq);

				CreateExplosion(*pSprite, 
					0.0f, 0.0f, 0.0f, // position
					0.0f, 0.0f, 0.0f, // radius
					25.0f, 5.0f,      // size
					0.0f, 0.0f);      // time            

				pRes = new SFResAni(pSprite);
			}
		}
		break;

	case EXPLSION_EFFECT_2:
		{
			CAnimSequence* pSeq = new CAnimSequence(SFSystem::GetInstance()->GetD3DDevice());

			if (false == pSeq->AddFrame(L"./media/Explosion_Frame01.dds", 0.03f) ||
				false == pSeq->AddFrame(L"./media/Explosion_Frame02.dds", 0.03f) ||
				false == pSeq->AddFrame(L"./media/Explosion_Frame03.dds", 0.03f) ||
				false == pSeq->AddFrame(L"./media/Explosion_Frame04.dds", 0.03f) ||
				false == pSeq->AddFrame(L"./media/Explosion_Frame05.dds", 0.03f) ||
				false == pSeq->AddFrame(L"./media/Explosion_Frame06.dds", 0.03f) ||
				false == pSeq->AddFrame(L"./media/Explosion_Frame07.dds", 0.03f) ||
				false == pSeq->AddFrame(L"./media/Explosion_Frame08.dds", 0.03f))
			{
				assert(0);
			}
			else
			{
				CSprite* pSprite = new CSprite(pSeq);

				CreateExplosion(*pSprite, 
					0.0f, 0.0f, 0.0f, // position
					0.0f, 0.0f, 0.0f, // radius
					25.0f, 5.0f,      // size
					0.0f, 0.0f);      // time            

				pRes = new SFResAni(pSprite);
			}
		}
		break;

	case EXPLSION_EFFECT_3:
		{
			CAnimSequence* pSeq = new CAnimSequence(SFSystem::GetInstance()->GetD3DDevice());

			if (false == pSeq->AddFrame(L"./media/SHINE0.bmp", 0.03f) ||
				false == pSeq->AddFrame(L"./media/SHINE1.bmp", 0.03f) ||
				false == pSeq->AddFrame(L"./media/SHINE2.bmp", 0.03f) ||
				false == pSeq->AddFrame(L"./media/SHINE3.bmp", 0.03f) ||
				false == pSeq->AddFrame(L"./media/SHINE4.bmp", 0.03f) ||
				false == pSeq->AddFrame(L"./media/SHINE5.bmp", 0.03f) ||
				false == pSeq->AddFrame(L"./media/SHINE6.bmp", 0.03f) ||
				false == pSeq->AddFrame(L"./media/SHINE7.bmp", 0.03f) ||
				false == pSeq->AddFrame(L"./media/SHINE8.bmp", 0.03f) ||
				false == pSeq->AddFrame(L"./media/SHINE9.bmp", 0.03f))
			{
				assert(0);
			}
			else
			{
				CSprite* pSprite = new CSprite(pSeq);

				CreateExplosion(*pSprite, 
					0.0f, 0.0f, 0.0f, // position
					0.0f, 0.0f, 0.0f, // radius
					25.0f, 5.0f,      // size
					0.0f, 0.0f);      // time            

				pRes = new SFResAni(pSprite);
			}
		}
		break;

	case MISAILE_1:
		{
			CAnimSequence* pSeq = new CAnimSequence(SFSystem::GetInstance()->GetD3DDevice());

			if (false == pSeq->AddFrame(L"./media/MISAILE1.bmp", 0.03f))
			{
				assert(0);
			}
			else
			{
				CSprite* pSprite = new CSprite(pSeq);

				CreateExplosion(*pSprite, 
					0.0f, 0.0f, 0.0f, // position
					0.0f, 0.0f, 0.0f, // radius
					//25.0f, 5.0f,      // size
					5.0f, 0.0f,      // size
					0.0f, 0.0f);      // time            

				pRes = new SFResAni(pSprite);
			}
		}
		break;

	case MISAILE_2:
		{
			CAnimSequence* pSeq = new CAnimSequence(SFSystem::GetInstance()->GetD3DDevice());

			if (false == pSeq->AddFrame(L"./media/MISAILE2.bmp", 0.03f))
			{
				assert(0);
			}
			else
			{
				CSprite* pSprite = new CSprite(pSeq);

				CreateExplosion(*pSprite, 
					0.0f, 0.0f, 0.0f, // position
					0.0f, 0.0f, 0.0f, // radius
					25.0f, 5.0f,      // size
					0.0f, 0.0f);      // time            

				pRes = new SFResAni(pSprite);
			}
		}
		break;
	}

	if(pRes)
		pRes->Init();

	return pRes;
}