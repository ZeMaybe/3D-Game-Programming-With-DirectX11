
#include "CrateApp.h"
#include "GeometryGenerator.h"
#include "WICTextureLoader.h"

using namespace DirectX;

CrateApp theApp;

CrateApp::CrateApp()
{
	mMainWndCaption = L"Crate Demo";

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mBoxWorld, I);
	XMStoreFloat4x4(&mTexTransform, I);

	mDirLights[0].Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
	mDirLights[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

	mDirLights[1].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(1.4f, 1.4f, 1.4f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.707f, 0.0f, 0.707f);

	mBoxMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mBoxMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mBoxMat.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
}

CrateApp::~CrateApp()	
{
	ReleaseCOM(mBoxVB);
	ReleaseCOM(mBoxIB);
	ReleaseCOM(mDiffuseMapSRV);
	ReleaseCOM(mInputLayout);
	SafeDelete(mEffect);
} 

bool CrateApp::Init(HINSTANCE hInstance)
{
	if (!D3DApp::Init(hInstance))
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	mEffect = new BasicTextureEffect(md3dDevice, L"../FX/BasicTexture.fxo");
	mInputLayout = mInputLayouts.InitLayout(md3dDevice,mEffect->Light1Tech , L"PosNormalTexCod");

	ID3D11Resource* res = 0; 
	HR(CreateWICTextureFromFile(md3dDevice, L"../Textures/WoodCrate01.dds", &res, &mDiffuseMapSRV));
	BuildGeometryBuffers();
	ReleaseCOM(res);

	return true;
} 

void CrateApp::DrawScene()
{
	D3DApp::DrawScene();

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = view*proj;
	
	mEffect->SetDirLights(mDirLights);
	mEffect->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTech = mEffect->Light2TexTech; 
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	UINT stride = sizeof(Vertex::PosNormalTexCod);
	UINT offset = 0;	

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the box.
		XMMATRIX world = XMLoadFloat4x4(&mBoxWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		mEffect->SetWorld(world);
		mEffect->SetWorldInvTranspose(worldInvTranspose);
		mEffect->SetWorldViewProj(worldViewProj);
		mEffect->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
		mEffect->SetMaterial(mBoxMat);
		mEffect->SetDiffuseMap(mDiffuseMapSRV);

		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);
	}

	HR(mSwapChain->Present(0, 0)); 
}

void CrateApp::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mBoxVertexOffset = 0;

	// Cache the index count of each object.
	mBoxIndexCount = box.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	mBoxIndexOffset = 0;

	UINT totalVertexCount = box.Vertices.size();

	UINT totalIndexCount = mBoxIndexCount;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<Vertex::PosNormalTexCod> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].Tex = box.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::PosNormalTexCod) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB)); 
}
