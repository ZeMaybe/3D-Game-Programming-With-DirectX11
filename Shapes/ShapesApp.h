
#ifndef ShapesApp_h__
#define ShapesApp_h__

#include "D3DApp.h" 
#include "D3DEffects.h"
#include "Vertex.h"
 
class ShapesApp : public D3DApp
{
public:
	ShapesApp();
	~ShapesApp();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildGeometryBuffers();
	void BuildRasterizerState();

private:
	ID3D11Buffer* mVB = 0;
	ID3D11Buffer* mIB = 0;

	PosColorEffect* mEffect;
	ID3D11InputLayout* mInputLayout =0;
	InputLayouts mInputLayouts;
	ID3D11RasterizerState* mWireframeRS =0;

	// Define transformations from local spaces to world space.
	DirectX::XMFLOAT4X4 mSphereWorld[10];
	DirectX::XMFLOAT4X4 mCylWorld[10];
	DirectX::XMFLOAT4X4 mGridWorld;
	DirectX::XMFLOAT4X4 mBoxWorld;
	DirectX::XMFLOAT4X4 mCenterSphereWorld;

	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	int mBoxVertexOffset;
	int mGridVertexOffset;
	int mSphereVertexOffset;
	int mCylinderVertexOffset;

	UINT mBoxIndexOffset;
	UINT mGridIndexOffset;
	UINT mSphereIndexOffset;
	UINT mCylinderIndexOffset;

	UINT mBoxIndexCount;
	UINT mGridIndexCount;
	UINT mSphereIndexCount;
	UINT mCylinderIndexCount;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};

#endif // ShapesApp_h__