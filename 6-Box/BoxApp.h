
#ifndef BoxApp_h__
#define BoxApp_h__

#include "D3DApp.h"
#include "D3DEffects.h"
#include "Vertex.h"


class BoxApp :public D3DApp
{
public:
	BoxApp();
	~BoxApp();

	bool Init(HINSTANCE hinst);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildGeometryBuffers();

private:
	ID3D11Buffer* mBoxVB = 0;
	ID3D11Buffer* mBoxIB = 0;

	PosColorEffect* mEffect = 0;
	ID3D11InputLayout* mInputLayout = 0;
	InputLayouts mInputLayouts;

	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};





#endif // BoxApp_h__