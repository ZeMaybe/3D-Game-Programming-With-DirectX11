
#ifndef Effects_h__
#define Effects_h__

#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "d3dx11effect.h"
#include "LightHelper.h"

class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& fileName);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX =0;
};

// 顶点颜色
class PosColorEffect : public Effect
{
public:
	PosColorEffect(ID3D11Device* device, const std::wstring& fileName);
	~PosColorEffect();

	void SetWorldViewProj(DirectX::CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique* ColorTech = 0;
	ID3DX11EffectMatrixVariable* WorldViewProj = 0;
};
 
// 基础灯光,三盏方向灯
class BasicDirLightsEffect : public Effect 
{
public:
	BasicDirLightsEffect(ID3D11Device* device, const std::wstring& fileName);
	~BasicDirLightsEffect();

	void SetWorldViewProj(DirectX::CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(DirectX::CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	ID3DX11EffectTechnique* Light1Tech=0;
	ID3DX11EffectTechnique* Light2Tech=0;
	ID3DX11EffectTechnique* Light3Tech=0;

	ID3DX11EffectMatrixVariable* WorldViewProj=0;
	ID3DX11EffectMatrixVariable* World=0;
	ID3DX11EffectMatrixVariable* WorldInvTranspose=0;
	ID3DX11EffectVectorVariable* EyePosW=0;
	ID3DX11EffectVariable* DirLights=0;
	ID3DX11EffectVariable* Mat=0;
};

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	//template<typename T>
	//static T* InitEffect(ID3D11Device* device, std::wstring filename);

	static BasicDirLightsEffect* BasicDirLightsFX;

private:
	//static std::vector<Effect*> mEffects;
}; 

//template<typename T>
//T* Effects::InitEffect(ID3D11Device* device, std::wstring filename)
//{
//	T* re = new T(device, filename);
//	mEffects.push_back(re);
//	return re;
//}

#endif // Effects_h__