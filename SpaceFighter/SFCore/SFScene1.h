#pragma once
#include "SFScene.h"

class SFScene1 : public SFScene
{
public:
	SFScene1(void);
	virtual ~SFScene1(void);

	virtual BOOL Init() override;
	virtual BOOL UpdateSpecificScene(float fElapsedTime) override;

protected:
	BOOL LoadGameObject();

private:
};
