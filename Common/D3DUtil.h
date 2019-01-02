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
#include <string>
#include <vector>

#include "DxErr.h"
#include "d3dx11effect.h" 
#include "MathHelper.h"

/*
#include "d3dx11Effect.h"
#include <dxerr.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <fstream>
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

class D3DHelper
{
public:
	static ID3D11ShaderResourceView* CreateTexture2DArraySRV(ID3D11Device* device,ID3D11DeviceContext* contex,
		std::vector<std::wstring>& fileNames,
		DXGI_FORMAT format = DXGI_FORMAT_B8G8R8X8_TYPELESS,
		UINT filter = 0,
		UINT mipFilter = 0);
};





#endif // D3DUtil_h__
