
#ifndef BasicTessellationApp_h__
#define BasicTessellationApp_h__

#include "D3DApp.h"
#include "Vertex.h"
#include "D3DEffects.h"

class BasicTessellationApp : public D3DApp
{
public:
	BasicTessellationApp();
	~BasicTessellationApp();

	bool Init(HINSTANCE hinst);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);

private:
	void BuildQuadPatchBuffer();

private:
	ID3D11Buffer* mQuadPatchVB = 0;

	TessellationEffect* mEffect = 0;
	ID3D11InputLayout* mInputLayout = 0;
	InputLayouts mInputLayouts;

	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	DirectX::XMFLOAT3 mEyePosW;


	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
}; 

#endif // BasicTessellationApp_h__