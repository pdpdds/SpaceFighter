#pragma once

class SFPlane;

class SFObjectFactory
{
public:
	SFObjectFactory(void);
	virtual ~SFObjectFactory(void);

	static SFPlane* CreateGameObject(int ObjectType);
};
