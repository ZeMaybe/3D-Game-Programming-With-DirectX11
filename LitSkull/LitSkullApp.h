#ifndef LitSkullApp_h__
#define LitSkullApp_h__

#include "D3DApp.h"
#include "LightHelper.h"
#include "D3DEffects.h"
#include "Vertex.h"

class LitSkullApp :public D3DApp
{
public:
	LitSkullApp();
	~LitSkullApp();

	bool Init(HINSTANCE hinst);
	void UpdateScene(float dt);
	void DrawScene();

private:
	void BuildShapeGeometryBuffers();
	void BuildSkullGeometryBuffers();

private:
	ID3D11Buffer* mShapesVB =0 ;
	ID3D11Buffer* mShapesIB = 0;

	ID3D11Buffer* mSkullVB = 0;
	ID3D11Buffer* mSkullIB = 0;

	DirectionalLight mDirLights[3];
	Material mGridMat;
	Material mBoxMat;
	Material mCylinderMat;
	Material mSphereMat;
	Material mSkullMat;

	// Define transformations from local spaces to world space.
	DirectX::XMFLOAT4X4 mSphereWorld[10];
	DirectX::XMFLOAT4X4 mCylWorld[10];
	DirectX::XMFLOAT4X4 mBoxWorld;
	DirectX::XMFLOAT4X4 mGridWorld;
	DirectX::XMFLOAT4X4 mSkullWorld;

	BasicDirLightsEffect* mEffect =0;
	ID3D11InputLayout* mLayout = 0;
	InputLayouts mLayouts;

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
	UINT mSkullIndexCount=0;

	UINT mLightCount=1;
};

#endif // LitSkullApp_h__
