
#ifndef BlurApp_h__
#define BlurApp_h__

#include "d3dApp.h"
#include "BlurFilter.h"
#include "Waves.h"
#include "LightHelper.h"
#include "Vertex.h"
#include "D3DEffects.h"

enum RenderOptions
{
	Lighting = 0,
	Textures = 1,
	TexturesAndFog = 2
};


class BlurApp : public D3DApp
{
public:
	BlurApp();
	~BlurApp();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void UpdateWaves();
	void DrawWrapper();
	void DrawScreenQuad();
	float GetHillHeight(float x, float z)const;
	DirectX::XMFLOAT3 GetHillNormal(float x, float z)const;
	void BuildLandGeometryBuffers();
	void BuildWaveGeometryBuffers();
	void BuildCrateGeometryBuffers();
	void BuildScreenQuadGeometryBuffers();
	void BuildOffscreenViews();

private:
	// 山
	ID3D11Buffer* mLandVB=0;
	ID3D11Buffer* mLandIB=0;

	// 水
	ID3D11Buffer* mWavesVB=0;
	ID3D11Buffer* mWavesIB=0;

	// 箱子
	ID3D11Buffer* mBoxVB=0;
	ID3D11Buffer* mBoxIB=0;

	// -- 屏幕 --
	ID3D11Buffer* mScreenQuadVB=0;
	ID3D11Buffer* mScreenQuadIB=0;

	// 草地，水体，木箱的贴图 -- 着色器资源视图
	ID3D11ShaderResourceView* mGrassMapSRV=0;
	ID3D11ShaderResourceView* mWavesMapSRV=0;
	ID3D11ShaderResourceView* mCrateSRV=0;

	ID3D11ShaderResourceView* mOffscreenSRV=0;
	ID3D11UnorderedAccessView* mOffscreenUAV=0;
	ID3D11RenderTargetView* mOffscreenRTV=0;

	//TextureBlendFogEffect
	ID3D11InputLayout* mInputLayout = 0;
	InputLayouts mInputLayouts;

	//BlurFilter mBlur;
	Waves mWaves;

	// 灯光与材质
	DirectionalLight mDirLights[3];
	Material mLandMat;
	Material mWavesMat;
	Material mBoxMat;

	DirectX::XMFLOAT4X4 mGrassTexTransform;
	DirectX::XMFLOAT4X4 mWaterTexTransform;
	DirectX::XMFLOAT4X4 mLandWorld;
	DirectX::XMFLOAT4X4 mWavesWorld;
	DirectX::XMFLOAT4X4 mBoxWorld;

	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	UINT mLandIndexCount=0;
	UINT mWaveIndexCount=0;

	DirectX::XMFLOAT2 mWaterTexOffset;

	RenderOptions mRenderOptions;

	DirectX::XMFLOAT3 mEyePosW;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
}; 

#endif // BlurApp_h__