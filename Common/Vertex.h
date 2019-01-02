
#ifndef Vertex_h__
#define Vertex_h__

#include "d3dx11effect.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <string>
#include <map>

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

	struct PosNormalTexCod
	{
		PosNormalTexCod();
		PosNormalTexCod(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& uv);
		PosNormalTexCod(float px, float py, float pz, float nx, float ny, float nz, float u, float v);

		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
	};

	struct TreePointSprite
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Size;
	};

}  

struct InputLayoutDesc
{
	InputLayoutDesc(const std::wstring& name, UINT s)
	{
		mLayoutName = name;
		mSize = s;
		mDesc = new D3D11_INPUT_ELEMENT_DESC[mSize];
	}

	~InputLayoutDesc()
	{
		delete[] mDesc;
	}
	
	std::wstring mLayoutName;
	UINT mSize;
	D3D11_INPUT_ELEMENT_DESC* mDesc;
};

typedef std::pair<std::wstring, InputLayoutDesc*> InputPair;
class InputLayoutFactory
{
public:
	InputLayoutFactory();
	~InputLayoutFactory();
	InputLayoutDesc* GetLayoutDescByName(const std::wstring& layoutName);

private:
	void InitAllInputDesc();
	void DestroyAllInputDesc();
	std::map<std::wstring, InputLayoutDesc*> mAllInputDesc;
}; 
 
class InputLayouts
{
public:
	InputLayouts() {}

	ID3D11InputLayout* InitLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech, std::wstring layoutName);

private:
	InputLayoutFactory mLayoutFactory;
};


#endif // Vertex_h__