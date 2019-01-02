
#ifndef MirrorApp_h__
#define MirrorApp_h__

#include "D3DApp.h" 
#include "LightHelper.h"
#include "Vertex.h"
#include "D3DEffects.h"

enum RenderOptions
{
	Lighting = 0,
	Textures = 1,
	TexturesAndFog = 2
};

class MirrorApp : public D3DApp
{
public:
	MirrorApp();
	~MirrorApp();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildRoomGeometryBuffers();
	void BuildSkullGeometryBuffers();

private:
	ID3D11Buffer* mRoomVB=0;

	ID3D11Buffer* mSkullVB=0;
	ID3D11Buffer* mSkullIB=0;

	ID3D11ShaderResourceView* mFloorDiffuseMapSRV=0;
	ID3D11ShaderResourceView* mWallDiffuseMapSRV=0;
	ID3D11ShaderResourceView* mMirrorDiffuseMapSRV=0;

	TextureBlendFogEffect* mEffect=0;
	ID3D11InputLayout* mInputLayout =0;
	InputLayouts mInputLayouts;

	DirectionalLight mDirLights[3];
	Material mRoomMat;
	Material mSkullMat;
	Material mMirrorMat;
	Material mShadowMat;

	DirectX::XMFLOAT4X4 mRoomWorld;
	DirectX::XMFLOAT4X4 mSkullWorld;

	UINT mSkullIndexCount =0;
	DirectX::XMFLOAT3 mSkullTranslation;

	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	RenderOptions mRenderOptions;

	DirectX::XMFLOAT3 mEyePosW;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};




#endif // MirrorApp_h__