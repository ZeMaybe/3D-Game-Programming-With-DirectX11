
#ifndef Vertex_h__
#define Vertex_h__

#include "d3dx11effect.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <string>

namespace Vertex
{
	struct PosNormal
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
	};

	struct PosColor
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
	}; 
}

#include <map>


struct InputLayouDesc
{
	InputLayouDesc(const std::wstring& name, UINT s)
	{
		mLayoutName = name;
		mSize = s;
		mDesc = new D3D11_INPUT_ELEMENT_DESC[mSize];
	}

	~InputLayouDesc()
	{
		delete[] mDesc;
	}
	
	std::wstring mLayoutName;
	UINT mSize;
	D3D11_INPUT_ELEMENT_DESC* mDesc;
};




class InputLayoutDesc
{
public:
	static const D3D11_INPUT_ELEMENT_DESC PosNormal[2];

	static const D3D11_INPUT_ELEMENT_DESC PosColor[2];

	//static const D3D11_INPUT_ELEMENT_DESC* GetLayoutDescByName(const std::wstring& layoutName);

	static std::map<std::wstring,const D3D11_INPUT_ELEMENT_DESC*> mAllInputDesc;

private:
	static void InitAllInputDesc();
};

class InputLayouts
{
public:
	static void InitAll(ID3D11Device* device,ID3DX11EffectTechnique* tech);
	static void DestroyAll();

	ID3D11InputLayout* InitLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech, std::wstring layoutName);

	static ID3D11InputLayout* PosNormal;
	static ID3D11InputLayout* PosColor;
};


#endif // Vertex_h__