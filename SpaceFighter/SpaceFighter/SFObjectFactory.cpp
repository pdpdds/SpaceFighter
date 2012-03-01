#include "StdAfx.h"
#include "SFObjectFactory.h"
#include "SFNormalPlane.h"
#include "SFBomb.h"
#include "SFTrigger.h"

SFObjectFactory::SFObjectFactory(void)
{
}

SFObjectFactory::~SFObjectFactory(void)
{
}

SFPlane* SFObjectFactory::CreateGameObject(int ObjectType)
{
	SFPlane* pObject = NULL;

	if(ObjectType == OBJECT_ENEMY_GENERAL)
	{
		pObject = new SFNormalPlane();
	}
	else if(ObjectType == OBJECT_BOMB)
	{
		pObject = new SFBomb();
	}
	else if(ObjectType == OBJECT_TRIGGER)
	{
		pObject = new SFTrigger();
	}

	return pObject;
}