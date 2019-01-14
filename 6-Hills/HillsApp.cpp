
#include "HillsApp.h"
#include "GeometryGenerator.h"
#include <fstream>
using namespace DirectX;

HillsApp theApp;

HillsApp::HillsApp()
{
	mMainWndCaption = L"Hills Demo";

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mGridWorld, I);
} 

HillsApp::~HillsApp()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	ReleaseCOM(mInputLayout);
	ReleaseCOM(mRasterizeStateWireFrame);

	SafeDelete(mEffect);
}

bool HillsApp::Init(HINSTANCE hInstance)
{
	if (!D3DApp::Init(hInstance))
		return false;

	BuildGeometryBuffers();
	mEffect = new PosColorEffect(md3dDevice, L"../FX/Color.fxo");
	mInputLayout = mInputLayouts.InitLayout(md3dDevice, mEffect->ColorTech, L"PosColor");

	mCam.SetPosition(0, 30, -30);
	return true;
}

void HillsApp::DrawScene()
{
	D3DApp::DrawScene();

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
	XMMATRIX worldViewProj = world*view*proj;

	UINT stride = sizeof(Vertex::PosColor);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	D3DX11_TECHNIQUE_DESC techDesc;
	mEffect->ColorTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the grid.
		mEffect->SetWorldViewProj(worldViewProj);
		mEffect->ColorTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, 0, 0);
	}

	HR(mSwapChain->Present(0, 0));
}

void HillsApp::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData grid;
	GeometryGenerator geoGen;
	geoGen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	mGridIndexCount = grid.Indices.size();

	std::vector<Vertex::PosColor> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;
		p.y = GetHeight(p.x, p.z);
		vertices[i].Pos = p;

		// Color the vertex based on its height.
		if (p.y < -10.0f)
		{
			// Sandy beach color.
			vertices[i].Color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
		}
		else if (p.y < 5.0f)
		{
			// Light yellow-green.
			vertices[i].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		}
		else if (p.y < 12.0f)
		{
			// Dark yellow-green.
			vertices[i].Color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		}
		else if (p.y < 20.0f)
		{
			// Dark brown.
			vertices[i].Color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		}
		else
		{
			// White snow.
			vertices[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::PosColor) * grid.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mGridIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &grid.Indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

float HillsApp::GetHeight(float x, float z) const
{
	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z)); 
}
