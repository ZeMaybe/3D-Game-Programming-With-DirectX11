
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
	void UpdateScene(float dt);
	void DrawScene(); 

private:
	float GetHillHeight(float x, float z)const;
	DirectX::XMFLOAT3 GetHillNormal(float x, float z)const;
	void BuildLandGeometryBuffers();
	void BuildWaveGeometryBuffers();

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

	DirPointSpotLightsEffect* mEffect=0;
	ID3D11InputLayout* mInputLayout = 0;
	InputLayouts mInputLayouts;
}; 

#endif // LightingApp_h__