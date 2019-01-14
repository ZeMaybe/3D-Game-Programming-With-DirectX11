
#ifndef BezierApp_h__
#define BezierApp_h__

#include "D3DApp.h"
#include "D3DEffects.h"
#include "Vertex.h"

class BezierApp : public D3DApp
{
public:
	BezierApp();
	~BezierApp();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

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
#endif // BezierApp_h__