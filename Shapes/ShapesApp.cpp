
#include "ShapesApp.h"
#include "GeometryGenerator.h"
#include <fstream>
using namespace DirectX;

ShapesApp theApp;

ShapesApp::ShapesApp() 
{
	mMainWndCaption = L"Shapes Demo";

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mGridWorld, I); 

	XMMATRIX boxScale = XMMatrixScaling(2.0f, 1.0f, 2.0f);
	XMMATRIX boxOffset = XMMatrixTranslation(0.0f, 0.5f, 0.0f);
	XMStoreFloat4x4(&mBoxWorld, XMMatrixMultiply(boxScale, boxOffset));

	XMMATRIX centerSphereScale = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	XMMATRIX centerSphereOffset = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&mCenterSphereWorld, XMMatrixMultiply(centerSphereScale, centerSphereOffset));

	for (int i = 0; i < 5; ++i)
	{
		XMStoreFloat4x4(&mCylWorld[i * 2 + 0], XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i*5.0f));
		XMStoreFloat4x4(&mCylWorld[i * 2 + 1], XMMatrixTranslation(+5.0f, 1.5f, -10.0f + i*5.0f));

		XMStoreFloat4x4(&mSphereWorld[i * 2 + 0], XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i*5.0f));
		XMStoreFloat4x4(&mSphereWorld[i * 2 + 1], XMMatrixTranslation(+5.0f, 3.5f, -10.0f + i*5.0f));
	} 
}

ShapesApp::~ShapesApp()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	ReleaseCOM(mInputLayout);
	ReleaseCOM(mWireframeRS);

	SafeDelete(mEffect);
}

bool ShapesApp::Init(HINSTANCE hInstance)
{
	if (!D3DApp::Init(hInstance))
		return false;

	BuildGeometryBuffers();
	mEffect = new PosColorEffect(md3dDevice, L"../FX/Color.fxo");
	mInputLayout = mInputLayouts.InitLayout(md3dDevice, mEffect->ColorTech, L"PosColor");
	BuildRasterizerState(); 

	return true;
} 
 
void ShapesApp::DrawScene()
{
	D3DApp::DrawScene();

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dImmediateContext->RSSetState(mWireframeRS);

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	UINT stride = sizeof(Vertex::PosColor);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	D3DX11_TECHNIQUE_DESC techDesc;
	mEffect->ColorTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the grid.
		XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
		mEffect->SetWorldViewProj(world*viewProj);
		mEffect->ColorTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);

		// Draw the box.
		world = XMLoadFloat4x4(&mBoxWorld);
		mEffect->SetWorldViewProj(world*viewProj);
		mEffect->ColorTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);

		// Draw center sphere.
		world = XMLoadFloat4x4(&mCenterSphereWorld);
		mEffect->SetWorldViewProj(world*viewProj);
		mEffect->ColorTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSphereIndexCount, mSphereIndexOffset, mSphereVertexOffset);

		// Draw the cylinders.
		for (int i = 0; i < 10; ++i)
		{
			world = XMLoadFloat4x4(&mCylWorld[i]);
			mEffect->SetWorldViewProj(world*viewProj);
			mEffect->ColorTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mCylinderIndexCount, mCylinderIndexOffset, mCylinderVertexOffset);
		}

		// Draw the spheres.
		for (int i = 0; i < 10; ++i)
		{
			world = XMLoadFloat4x4(&mSphereWorld[i]);
			mEffect->SetWorldViewProj(world*viewProj);
			mEffect->ColorTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mSphereIndexCount, mSphereIndexOffset, mSphereVertexOffset);
		}
	}

	HR(mSwapChain->Present(0, 0));
} 

void ShapesApp::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData grid;
	GeometryGenerator::MeshData box;
	GeometryGenerator::MeshData cylinder;
	GeometryGenerator::MeshData sphere;
	
	GeometryGenerator geoGen;
	geoGen.CreateGrid(20.0f, 30.0f, 60, 40, grid);
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);
	geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20, cylinder);
	geoGen.CreateSphere(0.5f, 20, 20, sphere);

	mGridVertexOffset = 0;
	mBoxVertexOffset = grid.Vertices.size();
	mCylinderVertexOffset = mBoxVertexOffset + box.Vertices.size();
	mSphereVertexOffset = mCylinderVertexOffset + cylinder.Vertices.size();
	
	mGridIndexCount = grid.Indices.size();
	mBoxIndexCount = box.Indices.size();
	mCylinderIndexCount = cylinder.Indices.size();
	mSphereIndexCount = sphere.Indices.size();

	mGridIndexOffset = 0;
	mBoxIndexOffset = mGridIndexCount;
	mCylinderIndexOffset = mBoxIndexOffset + mBoxIndexCount;
	mSphereIndexOffset = mCylinderIndexOffset + mCylinderIndexCount;

	UINT totalVertexCount = 
		grid.Vertices.size() 
		+ box.Vertices.size()
		+ cylinder.Vertices.size()
		+ sphere.Vertices.size();

	UINT totalIndexCount =
		mGridIndexCount +
		mBoxIndexCount +
		mCylinderIndexCount +
		mSphereIndexCount;

	std::vector<Vertex::PosColor> vertices(totalVertexCount);
	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

	UINT k = 0;
	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Color = black;
	}

	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Color = black;
	}

	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Color = black;
	}

	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Color = black;
	}
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::PosColor) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	std::vector<UINT> indices;
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());
	indices.insert(indices.end(), cylinder.Indices.begin(), cylinder.Indices.end());
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
} 

void ShapesApp::BuildRasterizerState()
{
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));
}
