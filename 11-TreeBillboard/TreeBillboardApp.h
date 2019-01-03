
#ifndef TreeBillboardApp_h__
#define TreeBillboardApp_h__

#include "D3DApp.h"
#include "LightHelper.h"
#include "D3DEffects.h"
#include "Vertex.h"
#include "Waves.h"

enum RenderOptions
{
	Lighting = 0,
	Textures = 1,
	TexturesAndFog = 2
};

class TreeBillboardApp : public D3DApp
{
public:
	TreeBillboardApp();
	~TreeBillboardApp();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	float GetHillHeight(float x, float z)const;
	DirectX::XMFLOAT3 GetHillNormal(float x, float z)const;
	void BuildLandGeometryBuffers();
	void BuildWaveGeometryBuffers();
	void BuildCrateGeometryBuffers();
	void BuildTreeSpritesBuffer();

	void DrawTreeSprites(DirectX::CXMMATRIX viewProj);

private:
	ID3D11Buffer* mLandVB=0;
	ID3D11Buffer* mLandIB=0;

	ID3D11Buffer* mWavesVB=0;
	ID3D11Buffer* mWavesIB=0;

	ID3D11Buffer* mBoxVB=0;
	ID3D11Buffer* mBoxIB=0;

	ID3D11Buffer* mTreeSpritesVB=0;

	ID3D11ShaderResourceView* mGrassMapSRV=0;
	ID3D11ShaderResourceView* mWavesMapSRV=0;
	ID3D11ShaderResourceView* mBoxMapSRV=0;
	ID3D11ShaderResourceView* mTreeTextureMapArraySRV=0;

	Waves mWaves;

	// effect
	TextureBlendFogEffect* mEffect01;    // 绘制山水
	TreeSpriteEffect* mEffect02;    // 绘制树

	InputLayouts mInputLayouts;
	ID3D11InputLayout* mInputLayout01;    // 绘制山水
	ID3D11InputLayout* mInputLayout02;    // 绘制树

	DirectionalLight mDirLights[3];
	Material mLandMat;
	Material mWavesMat;
	Material mBoxMat;
	Material mTreeMat;

	DirectX::XMFLOAT4X4 mGrassTexTransform;
	DirectX::XMFLOAT4X4 mWaterTexTransform;
	DirectX::XMFLOAT4X4 mLandWorld;
	DirectX::XMFLOAT4X4 mWavesWorld;
	DirectX::XMFLOAT4X4 mBoxWorld;

	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	UINT mLandIndexCount =0;

	static const UINT TreeCount = 15;

	bool mAlphaToCoverageOn = true;

	DirectX::XMFLOAT2 mWaterTexOffset;

	RenderOptions mRenderOptions;

	DirectX::XMFLOAT3 mEyePosW;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};


#endif // TreeBillboardApp_h__