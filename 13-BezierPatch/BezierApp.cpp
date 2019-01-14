
#include "BezierApp.h"
#include "RenderStates.h"

using namespace DirectX;

BezierApp theApp;

BezierApp::BezierApp()
{
	mMainWndCaption = L"Bezier Surface Demo";
}

BezierApp::~BezierApp()
{
	md3dImmediateContext->ClearState();
	ReleaseCOM(mQuadPatchVB);
	ReleaseCOM(mInputLayout);
	SafeDelete(mEffect);

	RenderStates::DestroyAll();
}

bool BezierApp::Init(HINSTANCE hInstance)
{
	if (!D3DApp::Init(hInstance))
		return false; 

	// Must init Effects first since InputLayouts depend on shader signatures.
	mEffect = new TessellationEffect(md3dDevice, L"../FX/BezierTessellation.fxo");
	mInputLayout = mInputLayouts.InitLayout(md3dDevice, mEffect->TessTech, L"OnlyPos");
	RenderStates::InitAll(md3dDevice);

	BuildQuadPatchBuffer();
	return true;
}

void BezierApp::DrawScene()
{
	D3DApp::DrawScene();

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST);

	UINT stride = sizeof(Vertex::OnlyPos);
	UINT offset = 0;
	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = view*proj;

	mEffect->SetEyePosW(mCam.GetPosition());
	
	D3DX11_TECHNIQUE_DESC techDesc;
	mEffect->TessTech->GetDesc(&techDesc);
	for (UINT p = 0;p < techDesc.Passes;++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mQuadPatchVB, &stride, &offset);

		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;
		mEffect->SetWorld(world);
		mEffect->SetWorldInvTranspose(worldInvTranspose);
		mEffect->SetWorldViewProj(worldViewProj);

		mEffect->TessTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		md3dImmediateContext->RSSetState(RenderStates::WireframeRS);
		md3dImmediateContext->Draw(16, 0);
	} 
	HR(mSwapChain->Present(0, 0));
}

void BezierApp::BuildQuadPatchBuffer()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * 16;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	XMFLOAT3 vertices[16] =
	{
		// Row 0
		XMFLOAT3(-10.0f, -10.0f, +15.0f),
		XMFLOAT3(-5.0f,  0.0f, +15.0f),
		XMFLOAT3(+5.0f,  0.0f, +15.0f),
		XMFLOAT3(+10.0f, 0.0f, +15.0f),

		// Row 1
		XMFLOAT3(-15.0f, 0.0f, +5.0f),
		XMFLOAT3(-5.0f,  0.0f, +5.0f),
		XMFLOAT3(+5.0f,  20.0f, +5.0f),
		XMFLOAT3(+15.0f, 0.0f, +5.0f),

		// Row 2
		XMFLOAT3(-15.0f, 0.0f, -5.0f),
		XMFLOAT3(-5.0f,  0.0f, -5.0f),
		XMFLOAT3(+5.0f,  0.0f, -5.0f),
		XMFLOAT3(+15.0f, 0.0f, -5.0f),

		// Row 3
		XMFLOAT3(-10.0f, 10.0f, -15.0f),
		XMFLOAT3(-5.0f,  0.0f, -15.0f),
		XMFLOAT3(+5.0f,  0.0f, -15.0f),
		XMFLOAT3(+25.0f, 10.0f, -15.0f)
	};

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mQuadPatchVB));
}
