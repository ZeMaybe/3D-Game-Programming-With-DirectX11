
#include "Effects.h"
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
