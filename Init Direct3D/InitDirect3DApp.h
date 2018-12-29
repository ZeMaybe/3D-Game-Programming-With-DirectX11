
 
#ifndef InitDirect3DApp_h__
#define InitDirect3DApp_h__

#include "D3DApp.h"

class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp();
	~InitDirect3DApp();

	bool Init(HINSTANCE hinst);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
}; 

#endif // InitDirect3DApp_h__


