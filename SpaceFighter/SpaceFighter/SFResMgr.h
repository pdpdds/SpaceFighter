#pragma once
#include <map>

class SFRes;

class SFResMgr
{
	typedef std::map<UINT, SFRes*> mapSFRes;
public:
	SFResMgr(void);
	virtual ~SFResMgr(void);

	BOOL AddResource(UINT resID);
	SFRes* FindResource(UINT resID);

protected:

private:
	mapSFRes m_mapSFRes;
};
