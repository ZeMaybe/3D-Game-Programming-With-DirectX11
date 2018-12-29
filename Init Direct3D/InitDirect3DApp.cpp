#include "InitDirect3DApp.h"
 

InitDirect3DApp theApp;

InitDirect3DApp::InitDirect3DApp()
{
	mMainWndCaption = L"InitDirect3DApp";
}

InitDirect3DApp::~InitDirect3DApp()
{
}

bool InitDirect3DApp::Init(HINSTANCE hinst)
{
	if (!D3DApp::Init(hinst))
		return false;

	return true;
}

void InitDirect3DApp::OnResize()
{
	D3DApp::OnResize(); 
}

void InitDirect3DApp::UpdateScene(float dt)
{

}

void InitDirect3DApp::DrawScene()
{
	//md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&DirectX::Colors::Green));
	//md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	D3DApp::DrawScene();

	HR(mSwapChain->Present(0, 0));
}
