#pragma once
#include "SFPlane.h"

class SFPlayerPlane : public SFPlane
{
public:
	SFPlayerPlane(void);
	virtual ~SFPlayerPlane(void);

	virtual BOOL Init(GameObjectDesc& Desc) override;
	virtual BOOL ProcessInput(float fElapsedTime) override;

	virtual BOOL Render(float fElapsedTime) override;
	virtual BOOL Update(float fElapsedTime) override;

	SFPlayerFSM* GetFSM(){return m_pFSM;}

protected:

private:

	SFPlayerFSM* m_pFSM;
};
