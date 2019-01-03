
#include "Vertex.h"
#include "D3DUtil.h" 

ID3D11InputLayout* InputLayouts::InitLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech, std::wstring layoutName)
{
	ID3D11InputLayout* re = 0;
	
	InputLayoutDesc* inputDesc = mLayoutFactory.GetLayoutDescByName(layoutName);

	if (inputDesc)
	{
		D3DX11_PASS_DESC passDesc;
		tech->GetPassByIndex(0)->GetDesc(&passDesc);
		HR(device->CreateInputLayout(inputDesc->mDesc, inputDesc->mSize,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize, &re));
	}

	return re;
}

InputLayoutFactory::InputLayoutFactory()
{
	InitAllInputDesc();
}

InputLayoutFactory::~InputLayoutFactory()
{
	DestroyAllInputDesc();
}

InputLayoutDesc* InputLayoutFactory::GetLayoutDescByName(const std::wstring& layoutName)
{
	return mAllInputDesc.at(layoutName);
}

void InputLayoutFactory::DestroyAllInputDesc()
{
	auto it = mAllInputDesc.begin();
	while (it != mAllInputDesc.end())
	{
		SafeDelete(it->second);
		++it;
	}
	mAllInputDesc.clear();
}

void InputLayoutFactory::InitAllInputDesc()
{
	InputLayoutDesc* PosNormal = new InputLayoutDesc(L"PosNormal", 2);
	PosNormal->mDesc[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
	PosNormal->mDesc[1] = { "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 };

	InputLayoutDesc* PosColor = new InputLayoutDesc(L"PosColor", 2);
	PosColor->mDesc[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 } ;
	PosColor->mDesc[1] = { "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 };

	InputLayoutDesc* PosNormalTexCod = new InputLayoutDesc(L"PosNormalTexCod", 3);
	PosNormalTexCod->mDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	PosNormalTexCod->mDesc[1] = { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	PosNormalTexCod->mDesc[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	InputLayoutDesc* TreePointSprite = new InputLayoutDesc(L"TreePointSprite", 2);
	TreePointSprite->mDesc[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
	TreePointSprite->mDesc[1] = { "SIZE",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 };
	
	mAllInputDesc.insert(InputPair(PosNormal->mLayoutName,PosNormal));
	mAllInputDesc.insert(InputPair(PosColor->mLayoutName, PosColor));
	mAllInputDesc.insert(InputPair(PosNormalTexCod->mLayoutName, PosNormalTexCod));
	mAllInputDesc.insert(InputPair(TreePointSprite->mLayoutName, TreePointSprite));
}

Vertex::PosNormalTexCod::PosNormalTexCod()
	:Pos(0.0f,0.0f,0.0f)
	,Normal(0.0f,0.0f,0.0f)
	,Tex(0.0f,0.0f)
{

}

Vertex::PosNormalTexCod::PosNormalTexCod(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
	:Pos(px,py,pz)
	,Normal(nx,ny,nz)
	,Tex(u,v)
{

}

Vertex::PosNormalTexCod::PosNormalTexCod(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& uv)
	:Pos(pos)
	,Normal(n)
	,Tex(uv)
{

}
