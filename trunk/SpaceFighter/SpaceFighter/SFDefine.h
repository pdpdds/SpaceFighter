#pragma once

#define SafeRelease(pInterface) if(pInterface != NULL) {pInterface->Release(); pInterface=NULL;}
#define SafeDelete(pObject) if(pObject != NULL) {delete pObject; pObject=NULL;}

#include <stdio.h>
#include <vector>
#include <mmsystem.h>
#include "D3DHelper.h"