
#ifndef BlurFilter_h__
#define BlurFilter_h__

#include "D3DUtil.h"
#include "D3DEffects.h"

class BlurFilter
{
public:
	BlurFilter();
	~BlurFilter();

	ID3D11ShaderResourceView* GetBlurredOutput();

	void SetGaussianWeights(float sigma);

	void SetWeights(const float weights[9]);

	void Init(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format);

	void BlurInPlace(ID3D11DeviceContext* dc, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* inputUAV, int blurCount);

private:
	int mWidth;
	int mHeight;
	DXGI_FORMAT mFormat;

	ID3D11ShaderResourceView* mBlurredOutputTexSRV = 0;
	ID3D11UnorderedAccessView* mBlurredOutputTexUAV = 0;

	BlurEffect* mEffect = 0;
};


#endif // BlurFilter_h__ 