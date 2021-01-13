// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include "SFDefine.h"
#include "DirectXEntity.h"
#include "SFConstant.h"
#include "SFStructure.h"
#include <assert.h>

#define gd3dDevice (SFSystem::GetInstance()->GetD3DDevice())
#define SFASSERT(x)  assert(x)

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "strmbasd.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")

#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#endif


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "d3dxof.lib")

#pragma comment(lib, "lua.lib")
#pragma comment(lib, "OrangeDump.lib")
#pragma comment(lib, "OrangeIni.lib")