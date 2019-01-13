
#include "BlurFilter.h"

BlurFilter::BlurFilter()
{

}

BlurFilter::~BlurFilter()
{
	ReleaseCOM(mBlurredOutputTexSRV);
	ReleaseCOM(mBlurredOutputTexUAV);
	SafeDelete(mEffect);
}

ID3D11ShaderResourceView* BlurFilter::GetBlurredOutput()
{
	return mBlurredOutputTexSRV;
}

void BlurFilter::SetGaussianWeights(float sigma)
{
	float d = 2.0f*sigma*sigma;
	float weights[9];
	float sum = 0.0f;

	for (int i = 0; i < 8; ++i)
	{
		float x = (float)i;
		weights[i] = expf(-x*x / d);

		sum += weights[i];
	}  

	for (int i = 0; i < 8; ++i)
	{
		weights[i] /= sum;
	}

	mEffect->SetWeights(weights);
}

void BlurFilter::SetWeights(const float weights[9])
{
	mEffect->SetWeights(weights);
}

void BlurFilter::Init(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	ReleaseCOM(mBlurredOutputTexSRV);
	ReleaseCOM(mBlurredOutputTexUAV);

	if (!mEffect)
	{
		mEffect = new BlurEffect(device, L"../FX/Blur.fxo");
	}

	mWidth = width;
	mHeight = height;
	mFormat = format;

	D3D11_TEXTURE2D_DESC blurredTexDesc;
	blurredTexDesc.Width = width;
	blurredTexDesc.Height = height;
	blurredTexDesc.MipLevels = 1;
	blurredTexDesc.ArraySize = 1;
	blurredTexDesc.Format = format;
	blurredTexDesc.SampleDesc.Count = 1;
	blurredTexDesc.SampleDesc.Quality = 0;
	blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
	blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	blurredTexDesc.CPUAccessFlags = 0;
	blurredTexDesc.MiscFlags = 0;

	ID3D11Texture2D* blurredTex = 0;
	HR(device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	HR(device->CreateShaderResourceView(blurredTex, &srvDesc, &mBlurredOutputTexSRV));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	HR(device->CreateUnorderedAccessView(blurredTex, &uavDesc, &mBlurredOutputTexUAV));

	ReleaseCOM(blurredTex);
}

void BlurFilter::BlurInPlace(ID3D11DeviceContext* dc, 
	ID3D11ShaderResourceView* inputSRV, 
	ID3D11UnorderedAccessView* inputUAV, 
	int blurCount)
{
	for (int i = 0; i < blurCount; ++i)
	{
		D3DX11_TECHNIQUE_DESC techDesc;
		mEffect->HorzBlurTech->GetDesc(&techDesc);
		for (UINT p = 0 ; p < techDesc.Passes;++p)
		{
			mEffect->SetInputMap(inputSRV);
			mEffect->SetOutputMap(mBlurredOutputTexUAV);
			mEffect->HorzBlurTech->GetPassByIndex(p)->Apply(0, dc);

			UINT numGroupsX = (UINT)ceilf(mWidth / 256.0f);
			dc->Dispatch(numGroupsX, mHeight, 1);
		}

		ID3D11ShaderResourceView* nullSRV[1] = { 0 };
		ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
		dc->CSSetShaderResources(0, 1, nullSRV);
		dc->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

		mEffect->VertBlurTech->GetDesc(&techDesc);
		for (UINT p = 0 ; p < techDesc.Passes;++p)
		{
			mEffect->SetInputMap(mBlurredOutputTexSRV);
			mEffect->SetOutputMap(inputUAV);
			mEffect->VertBlurTech->GetPassByIndex(p)->Apply(0, dc);
			
			UINT numGroupsY = (UINT)ceilf(mHeight / 256.0f);
			dc->Dispatch(mWidth, numGroupsY, 1);
		}
		dc->CSSetShaderResources(0, 1, nullSRV);
		dc->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	}
	dc->CSSetShader(0, 0, 0); 
}
