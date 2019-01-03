#ifndef RenderStates_h__
#define RenderStates_h__

#include <d3d11.h>
class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	// RasterizerState
	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* NoCullRS;
	static ID3D11RasterizerState* CullClockwiseRS;

	// BlendState
	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;
	static ID3D11BlendState* NoRenderTargetWritesBS;

	// Depth/stencil state
	static ID3D11DepthStencilState* MarkMirrorDSS;
	static ID3D11DepthStencilState* DrawReflectionDSS;
	static ID3D11DepthStencilState* NoDoubleBlendDSS;

private:
	static void InitRasterizerState(ID3D11Device* device);
	static void InitBlendState(ID3D11Device* device);
	static void InitDepthStencilState(ID3D11Device* device);
};


#endif // RenderStates_h__
