#pragma once

class GameObject;

class SFMessageReceiver
{
public:
	SFMessageReceiver(void);
	virtual ~SFMessageReceiver(void);

	virtual BOOL OnMessageEvent(GameObject* pSender, SFMessage& Msg){return FALSE;}
};