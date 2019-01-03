
#ifndef VecAddApp_h__
#define VecAddApp_h__
 
#include "d3dApp.h"
#include "D3DEffects.h"

struct Data
{
	DirectX::XMFLOAT3 v1;
	DirectX::XMFLOAT2 v2;
};

class VecAddApp : public D3DApp
{
public:
	VecAddApp();
	~VecAddApp();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void DoComputeWork();

private:
	void BuildBuffersAndViews();

private:
	ID3D11Buffer* mOutputBuffer=0;
	ID3D11Buffer* mOutputDebugBuffer=0;

	ID3D11ShaderResourceView* mInputASRV=0;
	ID3D11ShaderResourceView* mInputBSRV=0;
	ID3D11UnorderedAccessView* mOutputUAV=0;

	VecAddEffect* mEffect =0;


	UINT mNumElements;
};

#endif // VecAddApp_h__