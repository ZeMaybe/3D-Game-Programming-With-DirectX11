
#ifndef SkullApp_h__
#define SkullApp_h__

#include "D3DApp.h"
#include "D3DEffects.h"
#include "Vertex.h"

class SkullApp : public D3DApp
{
public:
	SkullApp();
	~SkullApp();

	bool Init(HINSTANCE hInstance);
	void DrawScene();

private:
	void BuildGeometryBuffers();
	void BuildRasterizerState();

private:
	ID3D11Buffer* mVB =0;
	ID3D11Buffer* mIB =0;

	PosColorEffect* mEffect =0;

	ID3D11InputLayout* mInputLayout=0;
	InputLayouts mInputLayouts;

	ID3D11RasterizerState* mWireframeRS=0;

	UINT mSkullIndexCount =0;

	// Define transformations from local spaces to world space.
	DirectX::XMFLOAT4X4 mSkullWorld;
};

#endif // SkullApp_h__