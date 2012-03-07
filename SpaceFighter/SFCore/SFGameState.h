#pragma once

class SFGameState
{
public:
	SFGameState(void);
	virtual ~SFGameState(void);

	virtual BOOL OnEnter(){return FALSE;}
	virtual BOOL OnLeave(){return FALSE;}

	virtual BOOL OnRender(float fElapsedTime){return FALSE;}
	virtual BOOL OnUpdate(float fTime, float fElapsedTime){return FALSE;}


protected:

private:
};