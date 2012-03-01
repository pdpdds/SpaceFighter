#pragma once

class SFBackGround : public CDirectXEntity
{
public:
	SFBackGround(void);
	virtual ~SFBackGround(void);

	virtual BOOL OnRender(float fElapsedTime) {return FALSE;}
	virtual BOOL OnResetDevice()  {return FALSE;}
	virtual BOOL OnFrameMove( double fTime, float fElapsedTime)  {return FALSE;}
	virtual BOOL OnLostDevice()  {return FALSE;}
	virtual BOOL OnDestroyDevice()  {return FALSE;}
	virtual BOOL Init() {return FALSE;}
};
