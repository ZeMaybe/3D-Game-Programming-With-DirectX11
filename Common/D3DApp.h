
#ifndef D3DApp_h__
#define D3DApp_h__

#include "D3DUtil.h"
#include "GameTimer.h"
#include "Camera.h"
#include <string>

class D3DApp
{
public:
	D3DApp();
	virtual ~D3DApp();

	static D3DApp* GetApplication();

	HINSTANCE AppInst()const;
	HWND      MainWnd()const;
	float     AspectRatio()const;

	int Run();

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.

	virtual bool Init(HINSTANCE hinst);
	virtual void OnResize();
	virtual void UpdateScene(float dt);
	virtual void DrawScene();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Convenience overrides for handling mouse input.
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

protected:
	bool InitMainWindow(HINSTANCE hInst);
	bool InitDirect3D();

	void CalculateFrameStats();

protected:

	HINSTANCE mhAppInst;
	HWND      mhMainWnd = 0;
	bool      mAppPaused = false;
	bool      mMinimized = false;
	bool      mMaximized = false;
	bool      mResizing = false;
	UINT      m4xMsaaQuality = 0;

	GameTimer mTimer;

	//Microsoft::WRL::ComPtr<ID3D11Device> md3dDevice;

	ID3D11Device* md3dDevice = 0;
	ID3D11DeviceContext* md3dImmediateContext = 0;
	IDXGISwapChain* mSwapChain = 0;
	ID3D11Texture2D* mDepthStencilBuffer = 0;
	ID3D11RenderTargetView* mRenderTargetView = 0;
	ID3D11DepthStencilView* mDepthStencilView = 0;
	D3D11_VIEWPORT mScreenViewport;

	// Derived class should set these in derived constructor to customize starting values.
	std::wstring mMainWndCaption;
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	int mClientWidth = 800;
	int mClientHeight = 600;
	bool mEnable4xMsaa = false;

	Camera mCam; 
	POINT mLastMousePos; 
private:
	static D3DApp* mApplication;
};



#endif // D3DApp_h__