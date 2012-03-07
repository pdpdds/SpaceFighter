#pragma once
#include <map>
#include "SFPlayerState.h"

class SFScene;

class SFPlayerFSM : public SFPlayerState
{
	typedef std::map<eSFPlayerState,SFPlayerState*> mapSFPlayerState;
public:
	SFPlayerFSM(SFPlayerPlane* pOwner);
	virtual ~SFPlayerFSM(void);

	BOOL AddState( eSFPlayerState state );
	BOOL ChangeState( eSFPlayerState state );

	virtual BOOL OnRender(float fElapsedTime) override;
	virtual BOOL OnUpdate(float fElapsedTime ) override;
	virtual BOOL ProcessInput(float fElapsedTime) override;
	virtual BOOL OnCheckInterAction(SFScene* pScene) override;

protected:

private:
	mapSFPlayerState m_mapSFPlayerState;
	SFPlayerState* m_pCurrentState;
};
