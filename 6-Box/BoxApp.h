
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
	void DrawScene();

private:
	void BuildGeometryBuffers();

private:
	ID3D11Buffer* mBoxVB = 0;
	ID3D11Buffer* mBoxIB = 0;

	PosColorEffect* mEffect = 0;
	ID3D11InputLayout* mInputLayout = 0;
	InputLayouts mInputLayouts;

	DirectX::XMFLOAT4X4 mWorld;
};

#endif // BoxApp_h__