
#ifndef InstancingAndCullingApp_h__
#define InstancingAndCullingApp_h__

#include "D3DApp.h"
#include "D3DEffects.h"
#include <DirectXCollision.h>

struct InstancedData
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4 Color;
};

class InstancingAndCullingApp : public D3DApp
{
public:
	InstancingAndCullingApp();
	~InstancingAndCullingApp();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
	  

private:
	void BuildSkullGeometryBuffers();
	void BuildInstancedBuffer(); 

private:
	ID3D11Buffer* mSkullVB=0;
	ID3D11Buffer* mSkullIB = 0;
	ID3D11Buffer* mInstancedBuffer = 0;

	//// Bounding box of the skull.
	//XNA::AxisAlignedBox mSkullBox;
	//XNA::Frustum mCamFrustum;

	DirectX::BoundingBox mSkullBox;
	DirectX::BoundingFrustum mCamFrustum;

	UINT mVisibleObjectCount =0;

	// Keep a system memory copy of the world matrices for culling.
	std::vector<InstancedData> mInstancedData;

	bool mFrustumCullingEnabled = true;

	DirectionalLight mDirLights[3];
	Material mSkullMat;

	// Define transformations from local spaces to world space.
	DirectX::XMFLOAT4X4 mSkullWorld;

	UINT mSkullIndexCount =0;

	//Camera mCam;
	//POINT mLastMousePos;
};

#endif  