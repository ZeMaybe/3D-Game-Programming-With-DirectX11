
#include "Vertex.h"
#include "D3DUtil.h"

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormal[2] =
{
	{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosColor[2]=
{
	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
};


//const D3D11_INPUT_ELEMENT_DESC* InputLayoutDesc::GetLayoutDescByName(const std::wstring& layoutName)
//{
//	if (mAllInputDesc.size() == 0)
//		InitAllInputDesc();
//
//	return mAllInputDesc.at(layoutName);
//}

std::map<std::wstring,const D3D11_INPUT_ELEMENT_DESC*> InputLayoutDesc::mAllInputDesc;

void InputLayoutDesc::InitAllInputDesc()
{
	mAllInputDesc.insert(std::pair<std::wstring, const D3D11_INPUT_ELEMENT_DESC*>(std::wstring(L"PosNormal"), &PosNormal[0]));
	mAllInputDesc.insert(std::pair<std::wstring, const D3D11_INPUT_ELEMENT_DESC*>(std::wstring(L"PosColor"), &PosColor[0]));
}

ID3D11InputLayout* InputLayouts::PosNormal =0;

ID3D11InputLayout* InputLayouts::PosColor =0;

void InputLayouts::InitAll(ID3D11Device* device, ID3DX11EffectTechnique* tech)
{
	D3DX11_PASS_DESC passDesc;
	tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::PosNormal, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &PosNormal));
}

void InputLayouts::DestroyAll()
{
	ReleaseCOM(PosNormal);
	ReleaseCOM(PosColor);
}

//ID3D11InputLayout* InputLayouts::InitLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech, std::wstring layoutName)
//{
//	
//}
