#pragma once

class SFPlayerPlane;
class SFScene;

class SFPlayerState
{
public:
	SFPlayerState(SFPlayerPlane* pOwner);
	virtual ~SFPlayerState(void);

	virtual BOOL OnEnter(){return FALSE;}
	virtual BOOL OnLeave(){return FALSE;}

	virtual BOOL ProcessInput(float fElapsedTime){return FALSE;}
	virtual BOOL OnRender(float fElapsedTime){return FALSE;}
	virtual BOOL OnUpdate(float fElapsedTime){return FALSE;}
	virtual BOOL OnCheckInterAction(SFScene* pScene){return FALSE;}

	SFPlayerPlane* GetOwner(){return m_pOwner;}

protected:

private:
	SFPlayerPlane* m_pOwner;
};
