
#ifndef BoxApp_h__
#define BoxApp_h__

#include "D3DApp.h"

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

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
	void BuildFX();
	void BuildVertexLayout();

private:
	ID3D11Buffer* mBoxVB = 0;
	ID3D11Buffer* mBoxIB = 0;

	ID3DX11Effect* mFx = 0;
	ID3DX11EffectTechnique* mTech = 0;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj = 0;

	ID3D11InputLayout* mInputLayout = 0;

	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};





#endif // BoxApp_h__