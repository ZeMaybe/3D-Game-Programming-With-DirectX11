
#include "D3DUtil.h"
#include "WICTextureLoader.h" 
#include "DDSTextureLoader.h"
using namespace DirectX;

ID3D11ShaderResourceView* D3DHelper::CreateTexture2DArraySRV(ID3D11Device* device, 
	ID3D11DeviceContext* contex,
	std::vector<std::wstring>& fileNames, 
	DXGI_FORMAT format /*= DXGI_FORMAT_FROM_FILE*/, 
	UINT filter /*= 0*/, 
	UINT mipFilter /*= 0*/)
{
	UINT size = fileNames.size();

	ID3D11ShaderResourceView* srv =0;

	std::vector<ID3D11Texture2D*> srcTex(size);
	for (UINT i = 0; i < size; ++i)
	{

		HR(CreateDDSTextureFromFileEx(device,
			fileNames[i].c_str(),
			512*512,
			D3D11_USAGE_STAGING,
			D3D11_BIND_SHADER_RESOURCE,
			D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE,
			0, true, (ID3D11Resource**)&srcTex[i], &srv));

		/*
			HRESULT __cdecl CreateDDSTextureFromFileEx(
		_In_ ID3D11Device* d3dDevice,
		_In_z_ const wchar_t* szFileName,
		_In_ size_t maxsize,
		_In_ D3D11_USAGE usage,
		_In_ unsigned int bindFlags,
		_In_ unsigned int cpuAccessFlags,
		_In_ unsigned int miscFlags,
		_In_ bool forceSRGB,
		_Outptr_opt_ ID3D11Resource** texture,
		_Outptr_opt_ ID3D11ShaderResourceView** textureView,
		_Out_opt_ DDS_ALPHA_MODE* alphaMode = nullptr);
		*/


		ReleaseCOM(srv); 
	}

	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	//ZeroMemory(&texArrayDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.ArraySize = size;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_STAGING;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	ID3D11Texture2D* texArray = 0;
	HR(device->CreateTexture2D(&texArrayDesc, 0, &texArray));

	for (UINT texElement = 0; texElement < size; ++texElement)
	{
		for (UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			HR(contex->Map(srcTex[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));

			contex->UpdateSubresource(texArray,
				D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels),
				0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

			contex->Unmap(srcTex[texElement], mipLevel);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;

	ID3D11ShaderResourceView* texArraySRV = 0;
	HR(device->CreateShaderResourceView(texArray, &viewDesc, &texArraySRV));

	ReleaseCOM(texArray);
	for (UINT i = 0; i < size; ++i)
	{
		ReleaseCOM(srcTex[i]);
	}

	return texArraySRV;
}
