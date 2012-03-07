#include "StdAfx.h"
#include "SFResMgr.h"
#include "SFSystem.h"
#include "SFRes.h"
#include "SFResFactory.h"

SFResMgr::SFResMgr(void)
{
}

SFResMgr::~SFResMgr(void)
{
}

BOOL SFResMgr::AddResource( UINT resID)
{
	mapSFRes::iterator iter = m_mapSFRes.find(resID);

	if(iter != m_mapSFRes.end())
		return FALSE;

	SFRes* pRes = SFResFactory::Create(resID);
	
	if(pRes == NULL)
		return FALSE;

	m_mapSFRes.insert(mapSFRes::value_type(resID, pRes));

	return TRUE;
}

SFRes* SFResMgr::FindResource( UINT resID )
{
	mapSFRes::iterator iter = m_mapSFRes.find(resID);

	if(iter != m_mapSFRes.end())
		return iter->second;

	return NULL;
}