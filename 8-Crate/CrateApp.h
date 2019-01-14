#ifndef CrateApp_h__
#define CrateApp_h__

#include "D3DApp.h"
#include "LightHelper.h"
#include "Vertex.h"
#include "D3DEffects.h"

class CrateApp : public D3DApp
{
public:
	CrateApp();
	~CrateApp();

	bool Init(HINSTANCE hInstance);
	void DrawScene();

private:
	void BuildGeometryBuffers();

private:
	ID3D11Buffer* mBoxVB=0;
	ID3D11Buffer* mBoxIB=0;
	 
	BasicTextureEffect* mEffect=0;
	InputLayouts mInputLayouts;
	ID3D11InputLayout* mInputLayout =0;

	// Ã˘Õº
	ID3D11ShaderResourceView* mDiffuseMapSRV=0;

	// π‚’’
	DirectionalLight mDirLights[3];
	Material mBoxMat;

	DirectX::XMFLOAT4X4 mTexTransform;
	DirectX::XMFLOAT4X4 mBoxWorld;

	int mBoxVertexOffset;
	UINT mBoxIndexOffset;
	UINT mBoxIndexCount;
};

#endif // CrateApp_h__
