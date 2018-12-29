

#ifndef HillsApp_h__
#define HillsApp_h__

#include "D3DApp.h"
#include "Effects.h"
#include "Vertex.h"
 
class HillsApp : public D3DApp
{

public:
	HillsApp();
	~HillsApp();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildGeometryBuffers();
	float GetHeight(float x, float z)const;

private:
	ID3D11Buffer* mVB = 0;
	ID3D11Buffer* mIB = 0;

	PosColorEffect* mEffect = 0;

	ID3D11InputLayout* mInputLayout = 0;
	InputLayouts mInputLayouts;
	ID3D11RasterizerState* mRasterizeStateWireFrame = 0;

	UINT mGridIndexCount = 0;
	
	// Define transformations from local spaces to world space.
	DirectX::XMFLOAT4X4 mGridWorld;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};



#endif // HillsApp_h__