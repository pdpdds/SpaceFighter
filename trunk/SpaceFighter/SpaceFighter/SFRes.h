#pragma once
#include "Timer.h"

class GameObject;
class OBB;

class SFRes
{
	friend class SFResMesh;
public:
	SFRes();
	virtual ~SFRes(void);

	virtual BOOL Render(float fElapsedTime) = 0;
	virtual BOOL Render(float fElapsedTime, GameObject* pObject){return FALSE;}
	virtual BOOL Render(float fElapsedTime, GameObject* pObject, CTimer& Timer){return FALSE;}
	virtual BOOL Init() = 0;

protected:

private:
	unsigned int m_ResType;
};
