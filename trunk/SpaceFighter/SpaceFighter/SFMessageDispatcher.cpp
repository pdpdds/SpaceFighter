#include "StdAfx.h"
#include "SFMessageDispatcher.h"
#include "SFSystem.h"
#include "SFScene.h"

namespace SFMessageDispatcher
{
	void MessageDispatch( GameObject* pObject, SFMessage& msg )
	{
		SFSystem* pSystem = (SFSystem*)SFSystem::GetInstance();
		SFScene* pScene = pSystem->GetCurrentScene();
		pScene->OnMessageEvent(pObject, msg);
	}
}