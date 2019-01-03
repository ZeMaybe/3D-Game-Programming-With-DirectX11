
#include "D3DEffects.h"
#include "D3DUtil.h"
#include <fstream>
#include <vector>
#include <assert.h>

Effect::Effect(ID3D11Device* device, const std::wstring& fileName)
{
	std::ifstream fin(fileName, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,0, device, &mFX));
}

Effect::~Effect()
{
	ReleaseCOM(mFX); 
}
 

BasicDirLightsEffect::BasicDirLightsEffect(ID3D11Device* device, const std::wstring& fileName)
	:Effect(device,fileName)
{
	Light1Tech = mFX->GetTechniqueByName("Light1");
	Light2Tech = mFX->GetTechniqueByName("Light2");
	Light3Tech = mFX->GetTechniqueByName("Light3");
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights = mFX->GetVariableByName("gDirLights");
	Mat = mFX->GetVariableByName("gMaterial");

	assert(Light1Tech->IsValid());
	assert(Light2Tech->IsValid());
	assert(Light3Tech->IsValid());
	assert(WorldViewProj->IsValid());
	assert(World->IsValid());
	assert(WorldInvTranspose->IsValid());
	assert(EyePosW->IsValid());
	assert(DirLights->IsValid());
	assert(Mat->IsValid()); 
}

BasicDirLightsEffect::~BasicDirLightsEffect()
{ 
	ReleaseCOM(WorldViewProj);
	ReleaseCOM(World);
	ReleaseCOM(WorldInvTranspose);
	ReleaseCOM(EyePosW);
	ReleaseCOM(DirLights);
	ReleaseCOM(Mat); 

	ReleaseCOM(Light1Tech);
	ReleaseCOM(Light2Tech);
	ReleaseCOM(Light3Tech);
}


void Effects::InitAll(ID3D11Device* device)
{
	BasicDirLightsFX = new BasicDirLightsEffect(device, L"../FX/BasicDirLights.fxo");
	DirPointSpotLightsFX = new DirPointSpotLightsEffect(device, L"../FX/DirPointSpotLights.fxo");
}

void Effects::DestroyAll()
{
	SafeDelete(BasicDirLightsFX);
	SafeDelete(DirPointSpotLightsFX);

	//for (size_t i = 0; i < mEffects.size(); ++i)
	//{
	//	SafeDelete(mEffects[i]);
	//}
} 

BasicDirLightsEffect* Effects::BasicDirLightsFX = 0;

DirPointSpotLightsEffect* Effects::DirPointSpotLightsFX = 0 ;

//std::vector<Effect*> Effects::mEffects;

PosColorEffect::PosColorEffect(ID3D11Device* device, const std::wstring& fileName)
	:Effect(device,fileName)
{
	ColorTech = mFX->GetTechniqueByName("ColorTech");
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();

	assert(ColorTech->IsValid());
	assert(WorldViewProj->IsValid());
}

PosColorEffect::~PosColorEffect()
{
	ReleaseCOM(ColorTech);
	ReleaseCOM(WorldViewProj);
}

DirPointSpotLightsEffect::DirPointSpotLightsEffect(ID3D11Device* device, const std::wstring& fileName)
	:Effect(device,fileName)
{
	LightTech = mFX->GetTechniqueByName("LightTech");
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLit = mFX->GetVariableByName("gDirLight");
	PointLit = mFX->GetVariableByName("gPointLight");
	SpotLit = mFX->GetVariableByName("gSpotLight");
	Mat = mFX->GetVariableByName("gMaterial");

	assert(LightTech->IsValid());
	assert(WorldViewProj->IsValid());
	assert(World->IsValid());
	assert(WorldInvTranspose->IsValid());
	assert(EyePosW->IsValid());
	assert(DirLit->IsValid());
	assert(PointLit->IsValid());
	assert(SpotLit->IsValid());
	assert(Mat->IsValid()); 
}

DirPointSpotLightsEffect::~DirPointSpotLightsEffect()
{
	ReleaseCOM(LightTech);
	ReleaseCOM(WorldViewProj);
	ReleaseCOM(World);
	ReleaseCOM(WorldInvTranspose);
	ReleaseCOM(EyePosW);
	ReleaseCOM(DirLit);
	ReleaseCOM(PointLit);
	ReleaseCOM(SpotLit);
	ReleaseCOM(Mat);
}

BasicTextureEffect::BasicTextureEffect(ID3D11Device* device, const std::wstring& fileName)
	:Effect(device,fileName)
{
	Light1Tech = mFX->GetTechniqueByName("Light1");
	Light2Tech = mFX->GetTechniqueByName("Light2");
	Light3Tech = mFX->GetTechniqueByName("Light3");

	Light0TexTech = mFX->GetTechniqueByName("Light0Tex");
	Light1TexTech = mFX->GetTechniqueByName("Light1Tex");
	Light2TexTech = mFX->GetTechniqueByName("Light2Tex");
	Light3TexTech = mFX->GetTechniqueByName("Light3Tex");

	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights = mFX->GetVariableByName("gDirLights");
	Mat = mFX->GetVariableByName("gMaterial");
	DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();

	assert(Light1Tech->IsValid());
	assert(Light2Tech->IsValid());
	assert(Light3Tech->IsValid());
	assert(Light0TexTech->IsValid());
	assert(Light1TexTech->IsValid());
	assert(Light2TexTech->IsValid());
	assert(Light3TexTech->IsValid());
	assert(WorldViewProj->IsValid());
	assert(World->IsValid());
	assert(WorldInvTranspose->IsValid());
	assert(TexTransform->IsValid());
	assert(EyePosW->IsValid());
	assert(DirLights->IsValid());
	assert(Mat->IsValid());
	assert(DiffuseMap->IsValid()); 
}

BasicTextureEffect::~BasicTextureEffect()
{
	ReleaseCOM(Light1Tech);
	ReleaseCOM(Light2Tech);
	ReleaseCOM(Light3Tech);
	ReleaseCOM(Light0TexTech);
	ReleaseCOM(Light1TexTech);
	ReleaseCOM(Light2TexTech);
	ReleaseCOM(Light3TexTech);
	ReleaseCOM(WorldViewProj);
	ReleaseCOM(World);
	ReleaseCOM(WorldInvTranspose);
	ReleaseCOM(TexTransform);
	ReleaseCOM(EyePosW);
	ReleaseCOM(DirLights);
	ReleaseCOM(DiffuseMap);  
}

TextureBlendFogEffect::TextureBlendFogEffect(ID3D11Device* device, const std::wstring& fileName)
	:BasicTextureEffect(device,fileName)
{
	Light0TexAlphaClipTech = mFX->GetTechniqueByName("Light0TexAlphaClip");
	Light1TexAlphaClipTech = mFX->GetTechniqueByName("Light1TexAlphaClip");
	Light2TexAlphaClipTech = mFX->GetTechniqueByName("Light2TexAlphaClip");
	Light3TexAlphaClipTech = mFX->GetTechniqueByName("Light3TexAlphaClip");

	Light1FogTech = mFX->GetTechniqueByName("Light1Fog");
	Light2FogTech = mFX->GetTechniqueByName("Light2Fog");
	Light3FogTech = mFX->GetTechniqueByName("Light3Fog");

	Light0TexFogTech = mFX->GetTechniqueByName("Light0TexFog");
	Light1TexFogTech = mFX->GetTechniqueByName("Light1TexFog");
	Light2TexFogTech = mFX->GetTechniqueByName("Light2TexFog");
	Light3TexFogTech = mFX->GetTechniqueByName("Light3TexFog");

	Light0TexAlphaClipFogTech = mFX->GetTechniqueByName("Light0TexAlphaClipFog");
	Light1TexAlphaClipFogTech = mFX->GetTechniqueByName("Light1TexAlphaClipFog");
	Light2TexAlphaClipFogTech = mFX->GetTechniqueByName("Light2TexAlphaClipFog");
	Light3TexAlphaClipFogTech = mFX->GetTechniqueByName("Light3TexAlphaClipFog");

	FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();

	assert(Light0TexAlphaClipTech->IsValid());
	assert(Light1TexAlphaClipTech->IsValid());
	assert(Light2TexAlphaClipTech->IsValid());
	assert(Light3TexAlphaClipTech->IsValid());
	assert(Light1FogTech->IsValid());
	assert(Light2FogTech->IsValid());
	assert(Light3FogTech->IsValid());
	assert(Light0TexFogTech->IsValid());
	assert(Light1TexFogTech->IsValid());
	assert(Light2TexFogTech->IsValid());
	assert(Light3TexFogTech->IsValid());
	assert(Light0TexAlphaClipFogTech->IsValid());
	assert(Light1TexAlphaClipFogTech->IsValid());
	assert(Light2TexAlphaClipFogTech->IsValid());
	assert(Light3TexAlphaClipFogTech->IsValid());
	assert(FogColor->IsValid());
	assert(FogStart->IsValid());
	assert(FogRange->IsValid()); 
}

TextureBlendFogEffect::~TextureBlendFogEffect()
{
	ReleaseCOM(Light0TexAlphaClipTech);
	ReleaseCOM(Light1TexAlphaClipTech);
	ReleaseCOM(Light2TexAlphaClipTech);
	ReleaseCOM(Light3TexAlphaClipTech);
	ReleaseCOM(Light1FogTech);
	ReleaseCOM(Light2FogTech);
	ReleaseCOM(Light3FogTech);
	ReleaseCOM(Light0TexFogTech);
	ReleaseCOM(Light1TexFogTech);
	ReleaseCOM(Light2TexFogTech);
	ReleaseCOM(Light3TexFogTech);
	ReleaseCOM(Light0TexAlphaClipFogTech);
	ReleaseCOM(Light1TexAlphaClipFogTech);
	ReleaseCOM(Light2TexAlphaClipFogTech);
	ReleaseCOM(Light3TexAlphaClipFogTech);
	ReleaseCOM(FogColor);
	ReleaseCOM(FogStart);
	ReleaseCOM(FogRange);
}

TreeSpriteEffect::TreeSpriteEffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device,filename)
{
	Light3Tech = mFX->GetTechniqueByName("Light3");
	Light3TexAlphaClipTech = mFX->GetTechniqueByName("Light3TexAlphaClip");
	Light3TexAlphaClipFogTech = mFX->GetTechniqueByName("Light3TexAlphaClipFog");

	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();
	DirLights = mFX->GetVariableByName("gDirLights");
	Mat = mFX->GetVariableByName("gMaterial");
	TreeTextureMapArray = mFX->GetVariableByName("gTreeMapArray")->AsShaderResource();

	assert(Light3Tech->IsValid());
	assert(Light3TexAlphaClipTech->IsValid());
	assert(Light3TexAlphaClipFogTech->IsValid());
	assert(ViewProj->IsValid());
	assert(EyePosW->IsValid());
	assert(FogColor->IsValid());
	assert(FogStart->IsValid());
	assert(FogRange->IsValid());
	assert(DirLights->IsValid());
	assert(Mat->IsValid());
	assert(TreeTextureMapArray->IsValid()); 
}

TreeSpriteEffect::~TreeSpriteEffect()
{
	ReleaseCOM(Light3Tech);
	ReleaseCOM(Light3TexAlphaClipTech);
	ReleaseCOM(Light3TexAlphaClipFogTech);
	ReleaseCOM(ViewProj);
	ReleaseCOM(EyePosW);
	ReleaseCOM(FogColor);
	ReleaseCOM(FogStart);
	ReleaseCOM(FogRange);
	ReleaseCOM(DirLights);
	ReleaseCOM(Mat);
	ReleaseCOM(TreeTextureMapArray);
}

VecAddEffect::VecAddEffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device,filename)
{
	VecAddTech = mFX->GetTechniqueByName("VecAdd");

	InputA = mFX->GetVariableByName("gInputA")->AsShaderResource();
	InputB = mFX->GetVariableByName("gInputB")->AsShaderResource();
	Output = mFX->GetVariableByName("gOutput")->AsUnorderedAccessView();

	assert(VecAddTech->IsValid());
	assert(InputA->IsValid());
	assert(InputB->IsValid());
	assert(Output->IsValid());

}

VecAddEffect::~VecAddEffect()
{
	ReleaseCOM(VecAddTech);
	ReleaseCOM(InputA);
	ReleaseCOM(InputB);
	ReleaseCOM(Output);
}
