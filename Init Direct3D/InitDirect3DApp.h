
 
#ifndef InitDirect3DApp_h__
#define InitDirect3DApp_h__

#include "D3DApp.h"

class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
}; 

#endif // InitDirect3DApp_h__


