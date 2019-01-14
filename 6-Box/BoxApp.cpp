
#include "BoxApp.h"

using namespace DirectX;

BoxApp theApp;

BoxApp::BoxApp()
{
	mMainWndCaption = L"Box Demo";

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
}

BoxApp::~BoxApp()
{
	ReleaseCOM(mBoxVB);
	ReleaseCOM(mBoxIB);

	ReleaseCOM(mInputLayout); 
	SafeDelete(mEffect);
}

bool BoxApp::Init(HINSTANCE hinst)
{
	if (!D3DApp::Init(hinst))
		return false;

	BuildGeometryBuffers();
	mEffect = new PosColorEffect(md3dDevice, L"../FX/Color.fxo");
	mInputLayout = mInputLayouts.InitLayout(md3dDevice, mEffect->ColorTech, L"PosColor");

	return true;
} 

void BoxApp::DrawScene()
{
	D3DApp::DrawScene();

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX worldViewProj = world * view*proj;

	UINT stride = sizeof(Vertex::PosColor);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);

	//mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
	mEffect->SetWorldViewProj(worldViewProj);

	D3DX11_TECHNIQUE_DESC techDesc;
	mEffect->ColorTech->GetDesc(&techDesc);
	//mTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mEffect->ColorTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		md3dImmediateContext->DrawIndexed(36, 0, 0);
	}

	HR(mSwapChain->Present(0, 0));
}
 
void BoxApp::BuildGeometryBuffers()
{
	// Create vertex buffer
	Vertex::PosColor vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::PosColor) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));

	// Create the index buffer 
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB));
}  