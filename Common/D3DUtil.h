#ifndef D3DUtil_h__
#define D3DUtil_h__

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <d3dcompiler.h>
#include <d3dcommon.h>

#include <wrl/client.h>

#include "DxErr.h"
#include "d3dx11effect.h" 
#include "MathHelper.h"

/*
#include "d3dx11Effect.h"
#include <dxerr.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "MathHelper.h"
#include "LightHelper.h"
*/

#define ReleaseCOM(p) { if(p) {p->Release(); p = 0;}}
#define SafeDelete(p) { if(p) {delete p;p= 0;}}

//---------------------------------------------------------------------------------------
// Simple d3d error checker for book demos.
//---------------------------------------------------------------------------------------

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#endif // D3DUtil_h__
