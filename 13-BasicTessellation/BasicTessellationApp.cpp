
#include "BasicTessellationApp.h"
#include "RenderStates.h"

using namespace DirectX; 

BasicTessellationApp theApp;

BasicTessellationApp::BasicTessellationApp()
{
	mMainWndCaption = L"Basic Tessellation Demo";
}

BasicTessellationApp::~BasicTessellationApp()
{
	md3dImmediateContext->ClearState();
	ReleaseCOM(mQuadPatchVB);

	ReleaseCOM(mInputLayout);
	SafeDelete(mEffect);
	RenderStates::DestroyAll();
}

bool BasicTessellationApp::Init(HINSTANCE hinst)
{
	if (!D3DApp::Init(hinst))
		return false;

	RenderStates::InitAll(md3dDevice);
	mEffect = new TessellationEffect(md3dDevice, L"../FX/Tessellation.fxo");
	mInputLayout = mInputLayouts.InitLayout(md3dDevice, mEffect->TessTech,L"OnlyPos");

	BuildQuadPatchBuffer(); 
	return true;
}
 
void BasicTessellationApp::DrawScene()
{
	D3DApp::DrawScene();

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = view*proj;

	// Set per frame constants.
	mEffect->SetEyePosW(mCam.GetPosition());

	UINT stride = sizeof(Vertex::OnlyPos);
	UINT offset = 0;
	D3DX11_TECHNIQUE_DESC techDesc;
	mEffect->TessTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mQuadPatchVB, &stride, &offset);

		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		mEffect->SetWorld(world);
		mEffect->SetWorldInvTranspose(worldInvTranspose);
		mEffect->SetWorldViewProj(worldViewProj);
		mEffect->SetTexTransform(XMMatrixIdentity());

		mEffect->TessTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		md3dImmediateContext->RSSetState(RenderStates::WireframeRS);
		md3dImmediateContext->Draw(4, 0);
	}

	HR(mSwapChain->Present(0, 0));
}
 
void BasicTessellationApp::BuildQuadPatchBuffer()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	XMFLOAT3 vertices[4] =
	{
		XMFLOAT3(-10.0f, 0.0f, +10.0f),
		XMFLOAT3(+10.0f, 0.0f, +10.0f),
		XMFLOAT3(-10.0f, 0.0f, -10.0f),
		XMFLOAT3(+10.0f, 0.0f, -10.0f)
	};

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mQuadPatchVB));
}
