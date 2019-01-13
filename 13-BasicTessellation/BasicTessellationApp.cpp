
#include "BasicTessellationApp.h"
#include "RenderStates.h"

using namespace DirectX; 

BasicTessellationApp theApp;

BasicTessellationApp::BasicTessellationApp()
	:mEyePosW(0.0f, 0.0f, 0.0f)
	, mTheta(1.3f*XM_PI)
	, mPhi(0.2f*XM_PI)
	, mRadius(80.0f)
{
	mMainWndCaption = L"Basic Tessellation Demo";
	mEnable4xMsaa = false;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
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


void BasicTessellationApp::OnResize()
{
	D3DApp::OnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*XM_PI, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}


void BasicTessellationApp::UpdateScene(float dt)
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	mEyePosW = XMFLOAT3(x, y, z);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);
}
void BasicTessellationApp::DrawScene()
{
	D3DApp::DrawScene();

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = view*proj;

	// Set per frame constants.
	mEffect->SetEyePosW(mEyePosW);
	mEffect->SetFogColor(Colors::Silver);
	mEffect->SetFogStart(15.0f);
	mEffect->SetFogRange(175.0f);

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
		mEffect->SetDiffuseMap(0);

		mEffect->TessTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		md3dImmediateContext->RSSetState(RenderStates::WireframeRS);
		md3dImmediateContext->Draw(4, 0);
	}

	HR(mSwapChain->Present(0, 0));
}

void BasicTessellationApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void BasicTessellationApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void BasicTessellationApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, XM_PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		float dx = 0.2f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.2f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 5.0f, 300.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
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
