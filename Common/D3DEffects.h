
#ifndef D3DEffects_h__
#define D3DEffects_h__

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

// 方向光,点光,聚光
class DirPointSpotLightsEffect : public Effect
{
public:
	DirPointSpotLightsEffect(ID3D11Device* device, const std::wstring& fileName);
	~DirPointSpotLightsEffect();

	void SetWorldViewProj(DirectX::CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(DirectX::CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDirLit(const DirectionalLight& lit) { DirLit->SetRawValue(&lit, 0, sizeof(DirectionalLight)); }
	void SetPointLit(const PointLight& lit) { PointLit->SetRawValue(&lit, 0, sizeof(PointLight)); }
	void SetSpotLit(const SpotLight& lit) { SpotLit->SetRawValue(&lit, 0, sizeof(SpotLight)); } 

	ID3DX11EffectTechnique* LightTech = 0;

	ID3DX11EffectMatrixVariable* WorldViewProj = 0;
	ID3DX11EffectMatrixVariable* World = 0;
	ID3DX11EffectMatrixVariable* WorldInvTranspose = 0;
	ID3DX11EffectVectorVariable* EyePosW = 0;
	ID3DX11EffectVariable* DirLit = 0;
	ID3DX11EffectVariable* PointLit = 0;
	ID3DX11EffectVariable* SpotLit = 0;
	ID3DX11EffectVariable* Mat = 0;
};

class BasicTextureEffect : public Effect
{
public:
	BasicTextureEffect(ID3D11Device* device, const std::wstring& fileName);
	~BasicTextureEffect();

	void SetWorldViewProj(DirectX::CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(DirectX::CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(DirectX::CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }

	ID3DX11EffectTechnique* Light1Tech=0;
	ID3DX11EffectTechnique* Light2Tech=0;
	ID3DX11EffectTechnique* Light3Tech=0;

	ID3DX11EffectTechnique* Light0TexTech=0;
	ID3DX11EffectTechnique* Light1TexTech=0;
	ID3DX11EffectTechnique* Light2TexTech=0;
	ID3DX11EffectTechnique* Light3TexTech=0;

	ID3DX11EffectMatrixVariable* WorldViewProj=0;
	ID3DX11EffectMatrixVariable* World=0;
	ID3DX11EffectMatrixVariable* WorldInvTranspose=0;
	ID3DX11EffectMatrixVariable* TexTransform=0;
	ID3DX11EffectVectorVariable* EyePosW=0;
	ID3DX11EffectVariable* DirLights=0;
	ID3DX11EffectVariable* Mat=0;

	ID3DX11EffectShaderResourceVariable* DiffuseMap=0;
};

class TextureBlendFogEffect : public BasicTextureEffect
{
public:
	TextureBlendFogEffect(ID3D11Device* device, const std::wstring& fileName);
	~TextureBlendFogEffect();

	void SetFogColor(const DirectX::FXMVECTOR v) { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f) { FogStart->SetFloat(f); }
	void SetFogRange(float f) { FogRange->SetFloat(f); } 

	ID3DX11EffectTechnique* Light0TexAlphaClipTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech; 

	ID3DX11EffectTechnique* Light1FogTech;
	ID3DX11EffectTechnique* Light2FogTech;
	ID3DX11EffectTechnique* Light3FogTech; 

	ID3DX11EffectTechnique* Light0TexFogTech;
	ID3DX11EffectTechnique* Light1TexFogTech;
	ID3DX11EffectTechnique* Light2TexFogTech;
	ID3DX11EffectTechnique* Light3TexFogTech; 

	ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange; 
};

class TreeSpriteEffect : public Effect
{
public:
	TreeSpriteEffect(ID3D11Device* device, const std::wstring& filename);
	~TreeSpriteEffect();

	void SetViewProj(DirectX::CXMMATRIX M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetFogColor(const DirectX::FXMVECTOR v) { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f) { FogStart->SetFloat(f); }
	void SetFogRange(float f) { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetTreeTextureMapArray(ID3D11ShaderResourceView* tex) { TreeTextureMapArray->SetResource(tex); }

	ID3DX11EffectTechnique* Light3Tech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* TreeTextureMapArray;
};



class VecAddEffect : public Effect
{
public:
	VecAddEffect(ID3D11Device* device, const std::wstring& filename);
	~VecAddEffect();

	void SetInputA(ID3D11ShaderResourceView* srv) { InputA->SetResource(srv); }
	void SetInputB(ID3D11ShaderResourceView* srv) { InputB->SetResource(srv); }
	void SetOutput(ID3D11UnorderedAccessView* uav) { Output->SetUnorderedAccessView(uav); }

	ID3DX11EffectTechnique* VecAddTech;

	ID3DX11EffectShaderResourceVariable* InputA;
	ID3DX11EffectShaderResourceVariable* InputB;
	ID3DX11EffectUnorderedAccessViewVariable* Output;
};


class BlurEffect : public Effect
{
public:
	BlurEffect(ID3D11Device* device, const std::wstring& filename);
	~BlurEffect();

	void SetWeights(const float weights[9]) { Weights->SetFloatArray(weights, 0, 9); }
	void SetInputMap(ID3D11ShaderResourceView* tex) { InputMap->SetResource(tex); }
	void SetOutputMap(ID3D11UnorderedAccessView* tex) { OutputMap->SetUnorderedAccessView(tex); }

	ID3DX11EffectTechnique* HorzBlurTech;
	ID3DX11EffectTechnique* VertBlurTech;

	ID3DX11EffectScalarVariable* Weights;
	ID3DX11EffectShaderResourceVariable* InputMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputMap;
};

class TessellationEffect : public Effect
{
public:
	TessellationEffect(ID3D11Device* device, const std::wstring& filename);
	~TessellationEffect();

	void SetWorldViewProj(DirectX::CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(DirectX::CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(DirectX::CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetFogColor(const DirectX::FXMVECTOR v) { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f) { FogStart->SetFloat(f); }
	void SetFogRange(float f) { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }

	ID3DX11EffectTechnique* TessTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
};

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();
 
	static BasicDirLightsEffect* BasicDirLightsFX;
	static DirPointSpotLightsEffect* DirPointSpotLightsFX;

private: 
}; 

 

#endif // Effects_h__