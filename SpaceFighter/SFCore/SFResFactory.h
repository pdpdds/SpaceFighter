#pragma once
#include "SFRes.h"
#include "Sprite.h"

class SFResFactory
{
public:
	SFResFactory(void);
	virtual ~SFResFactory(void);

	static SFRes* Create(unsigned int ResID);


	static void CreateExplosion(CSprite &spr,
		float PosX, float PosY, float PosZ,
		float RadiusX, float RadiusY, float RadiusZ,
		float Size, float SizeDelta,
		float Time, float TimeDelta);

protected:

private:
};
