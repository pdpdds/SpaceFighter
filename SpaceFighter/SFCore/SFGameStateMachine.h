#pragma once
#include <map>
#include "SFGameState.h"

class SFGameStateMachine : public SFGameState
{
	typedef std::map<eSFGameState,SFGameState*> mapSFGameState;
public:
	SFGameStateMachine(void);
	virtual ~SFGameStateMachine(void);

	BOOL AddState(eSFGameState state);
	BOOL ChangeState(eSFGameState state);

	virtual BOOL OnRender(float fElapsedTime) override;
	virtual BOOL OnUpdate(float fTime, float fElapsedTime) override;
	
protected:

private:
	SFGameState* m_pCurrentState;
	mapSFGameState m_mapSFGameState;
};
