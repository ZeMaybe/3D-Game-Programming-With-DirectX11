
#ifndef LightingApp_h__
#define LightingApp_h__

#include "D3DApp.h"
#include "Waves.h"
#include "LightHelper.h"
#include "D3DEffects.h"
#include "Vertex.h"

class LightingApp : public D3DApp
{
public:
	LightingApp();
	~LightingApp();

	bool Init(HINSTANCE hinst);
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
	//void BuildFX();
	//void BuildVertexLayout();

private:
	ID3D11Buffer* mLandVB = 0;
	ID3D11Buffer* mLandIB = 0;

	ID3D11Buffer* mWavesVB = 0;
	ID3D11Buffer* mWavesIB = 0; 

	Waves mWaves;
	Material mLandMat;
	Material mWavesMat;
	UINT mLandIndexCount;

	DirectionalLight mDirLight;
	PointLight mPointLight;
	SpotLight mSpotLight;

	DirectX::XMFLOAT4X4 mLandWorld;
	DirectX::XMFLOAT4X4 mWavesWorld;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	DirPointSpotLightsEffect* mEffect=0;
	ID3D11InputLayout* mInputLayout = 0;
	InputLayouts mInputLayouts;

	DirectX::XMFLOAT3 mEyePosW;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
}; 

#endif // LightingApp_h__