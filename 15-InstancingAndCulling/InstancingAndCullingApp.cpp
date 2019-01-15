
#include "InstancingAndCullingApp.h"
#include "Vertex.h"
#include <fstream>
#include <time.h>

using namespace DirectX;
 
//void InstancingAndCullingApp::DrawScene()
//{
//	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
//	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	md3dImmediateContext->IASetInputLayout(InputLayouts::InstancedBasic32);
//	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	UINT stride[2] = { sizeof(Vertex::Basic32), sizeof(InstancedData) };
//	UINT offset[2] = { 0,0 };
//
//	ID3D11Buffer* vbs[2] = { mSkullVB, mInstancedBuffer };
//
//	XMMATRIX view = mCam.View();
//	XMMATRIX proj = mCam.Proj();
//	XMMATRIX viewProj = mCam.ViewProj();
//
//	// Set per frame constants.
//	Effects::InstancedBasicFX->SetDirLights(mDirLights);
//	Effects::InstancedBasicFX->SetEyePosW(mCam.GetPosition());
//
//	ID3DX11EffectTechnique* activeTech = Effects::InstancedBasicFX->Light3Tech;
//
//	D3DX11_TECHNIQUE_DESC techDesc;
//	activeTech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		// Draw the skull.
//
//		md3dImmediateContext->IASetVertexBuffers(0, 2, vbs, stride, offset);
//		md3dImmediateContext->IASetIndexBuffer(mSkullIB, DXGI_FORMAT_R32_UINT, 0);
//
//		XMMATRIX world = XMLoadFloat4x4(&mSkullWorld);
//		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
//
//		Effects::InstancedBasicFX->SetWorld(world);
//		Effects::InstancedBasicFX->SetWorldInvTranspose(worldInvTranspose);
//		Effects::InstancedBasicFX->SetViewProj(viewProj);
//		Effects::InstancedBasicFX->SetMaterial(mSkullMat);
//
//		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//		md3dImmediateContext->DrawIndexedInstanced(mSkullIndexCount, mVisibleObjectCount, 0, 0, 0);
//	}
//
//	HR(mSwapChain->Present(0, 0));
//}
//
//

InstancingAndCullingApp::InstancingAndCullingApp()
{
	mMainWndCaption = L"Instancing and Culling Demo";

	srand((unsigned int)time((time_t *)NULL));

	XMMATRIX I = XMMatrixIdentity();
	XMMATRIX skullScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX skullOffset = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&mSkullWorld, XMMatrixMultiply(skullScale, skullOffset));

	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	mSkullMat.Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	mSkullMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mSkullMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
}

InstancingAndCullingApp::~InstancingAndCullingApp()
{
	ReleaseCOM(mSkullVB);
	ReleaseCOM(mSkullIB);
	ReleaseCOM(mInstancedBuffer);

//	Effects::DestroyAll();
//	InputLayouts::DestroyAll();
}

bool InstancingAndCullingApp::Init(HINSTANCE hInstance)
{
	if (!D3DApp::Init(hInstance))
		return false;

	//Effects::InitAll(md3dDevice);
	//InputLayouts::InitAll(md3dDevice);

	BuildSkullGeometryBuffers();
	BuildInstancedBuffer();

	return true;
}

void InstancingAndCullingApp::OnResize()
{
	D3DApp::OnResize();

	BoundingFrustum::CreateFromMatrix(mCamFrustum, mCam.Proj());
}

void InstancingAndCullingApp::UpdateScene(float dt)
{
	D3DApp::UpdateScene(dt);

	if (GetAsyncKeyState('1') & 0x8000)
		mFrustumCullingEnabled = true;

	if (GetAsyncKeyState('2') & 0x8000)
		mFrustumCullingEnabled = false;

	// Perform frustum culling.
	mCam.UpdateViewMatrix();
	mVisibleObjectCount = 0;

	if (mFrustumCullingEnabled)
	{
		XMVECTOR detView = XMMatrixDeterminant(mCam.View());
		XMMATRIX invView = XMMatrixInverse(&detView, mCam.View());

		D3D11_MAPPED_SUBRESOURCE mappedData;
		md3dImmediateContext->Map(mInstancedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		InstancedData* dataView = reinterpret_cast<InstancedData*>(mappedData.pData);

		for (UINT i = 0; i < mInstancedData.size(); ++i)
		{
			XMMATRIX w = XMLoadFloat4x4(&mInstancedData[i].World);
			XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(w), w);

			XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);



		}

	}


	//	if (mFrustumCullingEnabled)
	//	{
	//		for (UINT i = 0; i < mInstancedData.size(); ++i)
	//		{
	//
	//			// View space to the object's local space.
	//			XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);
	//
	//			// Decompose the matrix into its individual parts.
	//			XMVECTOR scale;
	//			XMVECTOR rotQuat;
	//			XMVECTOR translation;
	//			XMMatrixDecompose(&scale, &rotQuat, &translation, toLocal);
	//
	//			// Transform the camera frustum from view space to the object's local space.
	//			XNA::Frustum localspaceFrustum;
	//			XNA::TransformFrustum(&localspaceFrustum, &mCamFrustum, XMVectorGetX(scale), rotQuat, translation);
	//
	//			// Perform the box/frustum intersection test in local space.
	//			if (XNA::IntersectAxisAlignedBoxFrustum(&mSkullBox, &localspaceFrustum) != 0)
	//			{
	//				// Write the instance data to dynamic VB of the visible objects.
	//				dataView[mVisibleObjectCount++] = mInstancedData[i];
	//			}
	//		}
	//
	//		md3dImmediateContext->Unmap(mInstancedBuffer, 0);
	//	}
	//	else // No culling enabled, draw all objects.
	//	{
	//		D3D11_MAPPED_SUBRESOURCE mappedData;
	//		md3dImmediateContext->Map(mInstancedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	//
	//		InstancedData* dataView = reinterpret_cast<InstancedData*>(mappedData.pData);
	//
	//		for (UINT i = 0; i < mInstancedData.size(); ++i)
	//		{
	//			dataView[mVisibleObjectCount++] = mInstancedData[i];
	//		}
	//
	//		md3dImmediateContext->Unmap(mInstancedBuffer, 0);
	//	}
	//
	//	std::wostringstream outs;
	//	outs.precision(6);
	//	outs << L"Instancing and Culling Demo" <<
	//		L"    " << mVisibleObjectCount <<
	//		L" objects visible out of " << mInstancedData.size();
	//	mMainWndCaption = outs.str();
	//}
	//
}

void InstancingAndCullingApp::DrawScene()
{

}

void InstancingAndCullingApp::BuildSkullGeometryBuffers()
{
	std::ifstream fin("../Models/skull.txt");
	if (!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return;
	} 

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	XMFLOAT3 vMinf3(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	XMFLOAT3 vMaxf3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3); 

	std::vector<Vertex::PosNormalTexCod> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;

		XMVECTOR P = XMLoadFloat3(&vertices[i].Pos);
		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	XMStoreFloat3(&mSkullBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mSkullBox.Extents, 0.5f*(vMax - vMin));

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	mSkullIndexCount = 3 * tcount;
	std::vector<UINT> indices(mSkullIndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	} 
	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::PosNormalTexCod)*vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mSkullVB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)*mSkullIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mSkullIB));
} 

void InstancingAndCullingApp::BuildInstancedBuffer()
{
	const int n = 5;
	mInstancedData.resize(n*n*n);

	float width = 200.0f;
	float height = 200.0f;
	float depth = 200.0f;

	float x = -0.5f*width;
	float y = -0.5f*height;
	float z = -0.5f*depth;

	float dx = width / (n - 1);
	float dy = height / (n - 1);
	float dz = depth / (n - 1);

	for (int k = 0; k < n ;++k)
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				mInstancedData[k*n*n + i*n + j].World = XMFLOAT4X4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					x + j*dx, y + i*dy, z + k*dz, 1.0f
				);

				mInstancedData[k*n*n + i*n + j].Color.x = MathHelper::RandF(0.0f, 1.0f);
				mInstancedData[k*n*n + i*n + j].Color.y = MathHelper::RandF(0.0f, 1.0f);
				mInstancedData[k*n*n + i*n + j].Color.z = MathHelper::RandF(0.0f, 1.0f);
				mInstancedData[k*n*n + i*n + j].Color.w = 1.0f;
			}
		}
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(InstancedData)*mInstancedData.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	HR(md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer));
}
