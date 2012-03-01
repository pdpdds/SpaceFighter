#include "StdAfx.h"
#include "SFBackGroundParticle.h"
#include "SFSystem.h"
#include "SFResMgr.h"
#include "SFRes.h"
#include "SFCamera.h"
#include <list>

//the particle type
struct PARTICLE
{
	D3DXVECTOR3 Position ;
	D3DXVECTOR3 Velocity ;
	D3DXVECTOR3 Acceleration ;
	float Drag ;
	DWORD Color ;
	DWORD TimeLeft ;
} ;

//custom vertex contains xyz and diffuse
#define PARTICLE_VERTEX_FVF D3DFVF_XYZ | D3DFVF_DIFFUSE

struct ParticleVertex
{
	D3DVECTOR position ;
	DWORD color ;
} ;

//constants
//window class name
const char* WINDOWCLASS = "3D42DGP" ;
//window title
const char* WINDOWTITLE = "Example 16.1 (TGO-16-F): Particle System" ;

//particle count
const int PARTICLECOUNT = 1000 ;
//set the number of vertices for rendering 
const int PARTICLEVERTEXCOUNT = 100 ;

D3DVIEWPORT9 g_vpmain ;

//device type in use
D3DDEVTYPE g_devtype ;

using namespace std;
//the particle lists
list < PARTICLE* > lstParticlesAlive ;
list < PARTICLE* > lstParticlesDying ;
list < PARTICLE* > lstParticlesDead ;
//set up an array for particle vertices 
ParticleVertex g_vertParticles [ PARTICLEVERTEXCOUNT ] ;

SFBackGroundParticle::SFBackGroundParticle(void)
{
}

SFBackGroundParticle::~SFBackGroundParticle(void)
{
	//clean up particle lists
	list < PARTICLE* >::iterator iter ;
	PARTICLE* pParticle ;
	//loop through alive list
	for ( iter = lstParticlesAlive.begin ( ) ; iter != lstParticlesAlive.end ( ) ; iter ++ )
	{
		pParticle = *iter ;
		delete pParticle ;
	}
	for ( iter = lstParticlesDead.begin ( ) ; iter != lstParticlesDead.end ( ) ; iter ++ )
	{
		pParticle = *iter ;
		delete pParticle ;
	}
	lstParticlesAlive.clear ( ) ;
	lstParticlesDead.clear ( ) ;
	lstParticlesDying.clear ( ) ;
}

BOOL SFBackGroundParticle::OnRender( float fElapsedTime )
{
	RedrawFrame ( );


	return TRUE;
}

BOOL SFBackGroundParticle::OnResetDevice()
{
	return TRUE;
}

BOOL SFBackGroundParticle::OnFrameMove( double fTime, float fElapsedTime )
{	
	return TRUE;
}

BOOL SFBackGroundParticle::OnLostDevice()
{
	return TRUE;
}

BOOL SFBackGroundParticle::OnDestroyDevice()
{
	return TRUE;
}

BOOL SFBackGroundParticle::Init()
{
	m_pCamera = new CUserControlledCamera();
	m_pCamera->SetPosition(D3DXVECTOR3(0.0f, 80.0f, 0.0f));
	//m_pCamera->Update(0);

	m_pCamera->Rotation(0, PI / 2.0f ,0); 

	

	//set up particle lists
	int counter ;
	PARTICLE* pParticle ;
	for (counter = 0 ; counter < PARTICLECOUNT ; counter ++ )
	{
		pParticle = new PARTICLE ;
		lstParticlesDead.push_back ( pParticle ) ;
	}
	lstParticlesAlive.clear ( ) ;
	lstParticlesDying.clear ( ) ;

	D3DLIGHT9 light, light2;
	memset(&light, 0, sizeof(light));
	light.Ambient.a = 1.0f; light.Ambient.g = light.Ambient.b = light.Ambient.r = 1.0f;
	light.Diffuse.a = light.Diffuse.g = light.Diffuse.b = light.Diffuse.r =1.0f;
	light.Position = D3DXVECTOR3(0.0,80.0,0.0);
	light.Direction = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	//light.Type = D3DLIGHT_DIRECTIONAL;
	light.Type = D3DLIGHT_DIRECTIONAL;

	light2 = light;
	light2.Direction = D3DXVECTOR3(-1.0f,-1.0f,0.0f);

	gd3dDevice->SetLight(0, &light);
	gd3dDevice->LightEnable(0, TRUE);

	gd3dDevice->SetLight(1, &light2);
	gd3dDevice->LightEnable(1, TRUE);

	//set up viewports
	//main viewport
	g_vpmain.X = 0 ;
	g_vpmain.Y = 0 ;
	g_vpmain.Width = 640 ;
	g_vpmain.Height = 480 ;
	g_vpmain.MinZ = 0.0f ;
	g_vpmain.MaxZ = 1.0f ;

	//set the viewport
	gd3dDevice->SetViewport ( &g_vpmain ) ;

	//turn off lighting
	gd3dDevice->SetRenderState ( D3DRS_LIGHTING , FALSE ) ;

	return TRUE;
}

//redraw frame
void SFBackGroundParticle::RedrawFrame ( ) 
{
	//clear the screen
	gd3dDevice->Clear ( 0 , NULL , D3DCLEAR_TARGET , D3DCOLOR_XRGB ( 0 , 0 , 0 ) , 0 , 0 ) ;

	gd3dDevice->SetRenderState ( D3DRS_LIGHTING , FALSE ) ;

	//add a particle to the list
	list < PARTICLE* >::iterator iter ;
	PARTICLE* pParticle ;
	for ( int i = 0 ; i < 10 ; i ++ ) 
		//check for a particle in the dead list
		if ( !lstParticlesDead.empty ( ) ) 
		{
			//a particle exists in the dead list
			//grab the first one
			iter = lstParticlesDead.begin ( ) ;
			pParticle = *iter ;

			/*set up the particle here*/
			pParticle->Position.x = ( float ) ( rand ( ) % 1000 ) * 2.0 / 1000.0 - 1.0 ;
			pParticle->Position.y = 1.0 ;
			pParticle->Position.z = 0.0 ;
			pParticle->Velocity.x = 0.0 ;
			pParticle->Velocity.y = 0.0 ;
			pParticle->Velocity.z = 0.0 ;
			pParticle->Acceleration.x = 0.0 ;
			pParticle->Acceleration.y = ( float ) ( rand ( ) % 50 ) * -0.0001 -0.005 ;
			pParticle->Acceleration.z = 0.0 ;
			pParticle->Drag = 15.0/16.0 ;
			pParticle->Color = D3DCOLOR_XRGB ( 0 , 0 , 255 ) ;
			pParticle->TimeLeft = 100 ;

			//remove particle from dead list
			lstParticlesDead.remove ( pParticle ) ;
			//add particle to alive list
			lstParticlesAlive.push_back ( pParticle ) ;	
		}

		DWORD dwFvF;
		gd3dDevice->GetFVF(&dwFvF);

		//begin the scene
		gd3dDevice->BeginScene ( ) ;

		//make sure particle list is not empty
		if ( !lstParticlesAlive.empty ( ) ) 
		{
			//set vertex shader for particle vertices

			gd3dDevice->SetFVF( PARTICLE_VERTEX_FVF ) ;
			int nVertex = 0 ;
			//loop through the particles
			for ( iter = lstParticlesAlive.begin ( ) ; iter != lstParticlesAlive.end ( ) ; iter ++ )
			{
				pParticle = *iter ;
				//apply drag to velocity
				pParticle->Velocity *= pParticle->Drag ;
				//add acceleration to velocity
				pParticle->Velocity += pParticle->Acceleration ;
				//add velocity to position
				pParticle->Position += pParticle->Velocity ;
				//decrease the time left for this particle
				pParticle->TimeLeft -- ;
				//check for death
				if ( pParticle->TimeLeft == 0 )
				{
					//add this particle to the dying list
					lstParticlesDying.push_back ( pParticle ) ;
				}
				//add particle to vertex list
				g_vertParticles [ nVertex ].position = pParticle->Position ;
				g_vertParticles [ nVertex ].color = pParticle->Color ;
				//increment vertex number
				nVertex ++ ;
				//if we have filled up the array
				if ( nVertex == PARTICLEVERTEXCOUNT ) 
				{
					//draw all of the particles in the array
					gd3dDevice->DrawPrimitiveUP ( D3DPT_POINTLIST ,
						PARTICLEVERTEXCOUNT ,
						g_vertParticles ,
						sizeof ( ParticleVertex ) ) ;
					//set vertex index back to 0
					nVertex = 0 ;
				}
			}
			//render remaining particles, if any
			if ( nVertex != 0 ) 
			{
				//draw remaining particles (nVertex == the number remaining)
				gd3dDevice->DrawPrimitiveUP ( D3DPT_POINTLIST ,
					nVertex ,
					g_vertParticles ,
					sizeof ( ParticleVertex ) ) ;
			}
		}


		//end the scene
		gd3dDevice->EndScene ( ) ;

		gd3dDevice->SetFVF( dwFvF ) ;

		//if the dying list has particles in it
		if ( !lstParticlesDying.empty ( ) )
		{
			list < PARTICLE* >::iterator iter ;
			PARTICLE* pParticle ;
			//loop through all the particles on the list
			for ( iter = lstParticlesDying.begin ( ) ; iter != lstParticlesDying.end ( ) ; iter ++ )
			{
				pParticle = *iter ;
				//remove the particle from the live list
				lstParticlesAlive.remove ( pParticle ) ;
				//add the particle to the dead list
				lstParticlesDead.push_back ( pParticle ) ;
			}
			//clear out the dying list
			lstParticlesDying.clear ( ) ;
		}

		//gd3dDevice->SetRenderState ( D3DRS_LIGHTING , TRUE) ;
		//gd3dDevice->SetTransform( D3DTS_VIEW, &m_pCamera->GetViewMatrix());

}
