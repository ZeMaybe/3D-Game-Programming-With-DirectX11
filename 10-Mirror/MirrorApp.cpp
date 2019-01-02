
#include "MirrorApp.h"
#include "RenderStates.h"
#include "WICTextureLoader.h"
#include <fstream>
#include <vector>

using namespace DirectX;

MirrorApp theApp;



MirrorApp::MirrorApp()
	:mSkullTranslation(0.0f, 1.0f, -5.0f),
	mEyePosW(0.0f, 0.0f, 0.0f), 
	mRenderOptions(RenderOptions::Textures),
	mTheta(1.24f*XM_PI),
	mPhi(0.42f*XM_PI), 
	mRadius(12.0f)
{
	mMainWndCaption = L"Mirror Demo";

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mRoomWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

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

	mRoomMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mRoomMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mRoomMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

	mSkullMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mSkullMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSkullMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

	// Reflected material is transparent so it blends into mirror.
	mMirrorMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mMirrorMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	mMirrorMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

	mShadowMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mShadowMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	mShadowMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);
}


MirrorApp::~MirrorApp()
{
	ReleaseCOM(mRoomVB);
	ReleaseCOM(mSkullVB);
	ReleaseCOM(mSkullIB);
	ReleaseCOM(mFloorDiffuseMapSRV);
	ReleaseCOM(mWallDiffuseMapSRV);
	ReleaseCOM(mMirrorDiffuseMapSRV);

	md3dImmediateContext->ClearState();
	ReleaseCOM(mInputLayout);
	SafeDelete(mEffect);
	RenderStates::DestroyAll();
}


bool MirrorApp::Init(HINSTANCE hinst)
{
	if (!D3DApp::Init(hinst))
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	mEffect = new TextureBlendFogEffect(md3dDevice, L"../FX/BasicTextureFog.fxo");
	mInputLayout = mInputLayouts.InitLayout(md3dDevice, mEffect->Light1Tech, L"PosNormalTexCod");
	RenderStates::InitAll(md3dDevice);

	ID3D11Resource* res = 0;
	HR(CreateWICTextureFromFile(md3dDevice, L"../Textures/checkboard.dds", &res, &mFloorDiffuseMapSRV));
	ReleaseCOM(res);

	HR(CreateWICTextureFromFile(md3dDevice, L"../Textures/brick01.dds", &res, &mWallDiffuseMapSRV));
	ReleaseCOM(res);

	HR(CreateWICTextureFromFile(md3dDevice, L"../Textures/ice.dds", &res, &mMirrorDiffuseMapSRV));
	ReleaseCOM(res);

	BuildRoomGeometryBuffers();
	BuildSkullGeometryBuffers();

	return true;
} 

void MirrorApp::OnResize()
{
	D3DApp::OnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*XM_PI, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}


void MirrorApp::UpdateScene(float dt)
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

	// Switch the render mode based in key input.
	if (GetAsyncKeyState('1') & 0x8000)
		mRenderOptions = RenderOptions::Lighting;

	if (GetAsyncKeyState('2') & 0x8000)
		mRenderOptions = RenderOptions::Textures;

	if (GetAsyncKeyState('3') & 0x8000)
		mRenderOptions = RenderOptions::TexturesAndFog;


	// Allow user to move box.
	if (GetAsyncKeyState('A') & 0x8000)
		mSkullTranslation.x -= 1.0f*dt;

	if (GetAsyncKeyState('D') & 0x8000)
		mSkullTranslation.x += 1.0f*dt;

	if (GetAsyncKeyState('W') & 0x8000)
		mSkullTranslation.y += 1.0f*dt;

	if (GetAsyncKeyState('S') & 0x8000)
		mSkullTranslation.y -= 1.0f*dt;

	// Don't let user move below ground plane.
	mSkullTranslation.y = MathHelper::Max(mSkullTranslation.y, 0.0f);

	// Update the new world matrix.
	XMMATRIX skullRotate = XMMatrixRotationY(0.5f*XM_PI);
	XMMATRIX skullScale = XMMatrixScaling(0.45f, 0.45f, 0.45f);
	XMMATRIX skullOffset = XMMatrixTranslation(mSkullTranslation.x, mSkullTranslation.y, mSkullTranslation.z);
	XMStoreFloat4x4(&mSkullWorld, skullRotate*skullScale*skullOffset);
}

void MirrorApp::DrawScene()
{
	D3DApp::DrawScene();

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = view*proj;

	// Set per frame constants.
	mEffect->SetDirLights(mDirLights);
	mEffect->SetEyePosW(mEyePosW);
	mEffect->SetFogColor(Colors::Black);
	mEffect->SetFogStart(2.0f);
	mEffect->SetFogRange(40.0f);

	// Skull doesn't have texture coordinates, so we can't texture it.
	ID3DX11EffectTechnique* activeTech;
	ID3DX11EffectTechnique* activeSkullTech;

	switch (mRenderOptions)
	{
	case RenderOptions::Lighting:
		activeTech = mEffect->Light3Tech;
		activeSkullTech = mEffect->Light3Tech;
		break;
	case RenderOptions::Textures:
		activeTech = mEffect->Light3TexTech;
		activeSkullTech = mEffect->Light3Tech;
		break;
	case RenderOptions::TexturesAndFog:
		activeTech = mEffect->Light3TexFogTech;
		activeSkullTech = mEffect->Light3FogTech;
		break;
	}

	UINT stride = sizeof(Vertex::PosNormalTexCod);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	// Draw the floor and walls to the back buffer as normal.
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = activeTech->GetPassByIndex(p);

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mRoomVB, &stride, &offset);

		// Set per object constants.
		XMMATRIX world = XMLoadFloat4x4(&mRoomWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		mEffect->SetWorld(world);
		mEffect->SetWorldInvTranspose(worldInvTranspose);
		mEffect->SetWorldViewProj(worldViewProj);
		mEffect->SetTexTransform(XMMatrixIdentity());
		mEffect->SetMaterial(mRoomMat);

		// Floor  -- 绘制地板
		mEffect->SetDiffuseMap(mFloorDiffuseMapSRV);
		pass->Apply(0, md3dImmediateContext);
		md3dImmediateContext->Draw(6, 0);

		// Wall  -- 绘制墙壁
		mEffect->SetDiffuseMap(mWallDiffuseMapSRV);
		pass->Apply(0, md3dImmediateContext);
		md3dImmediateContext->Draw(18, 6);
	} 

	// Draw the skull to the back buffer as normal. -- 绘制骷髅
	activeSkullTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = activeSkullTech->GetPassByIndex(p);

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mSkullVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mSkullIB, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX world = XMLoadFloat4x4(&mSkullWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		mEffect->SetWorld(world);
		mEffect->SetWorldInvTranspose(worldInvTranspose);
		mEffect->SetWorldViewProj(worldViewProj);
		mEffect->SetMaterial(mSkullMat);

		pass->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);
	}

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f }; 

	// Draw the mirror to stencil buffer only.   -- 标记出镜子的模版
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = activeTech->GetPassByIndex(p);
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mRoomVB, &stride, &offset);

		// Set per object constants.
		XMMATRIX world = XMLoadFloat4x4(&mRoomWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		mEffect->SetWorld(world);
		mEffect->SetWorldInvTranspose(worldInvTranspose);
		mEffect->SetWorldViewProj(worldViewProj);
		mEffect->SetTexTransform(XMMatrixIdentity());

		md3dImmediateContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactor, 0xFFFFFFFF);
		md3dImmediateContext->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);

		pass->Apply(0, md3dImmediateContext);
		md3dImmediateContext->Draw(6, 24);

		md3dImmediateContext->OMSetDepthStencilState(0, 0);
		md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xFFFFFFFF);
	}
	
 
	// 绘制镜子里的骷髅
	activeSkullTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = activeSkullTech->GetPassByIndex(p);

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mSkullVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mSkullIB, DXGI_FORMAT_R32_UINT, 0);

		XMVECTOR mirrorPlane = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
		XMMATRIX R = XMMatrixReflect(mirrorPlane);
		XMMATRIX world = XMLoadFloat4x4(&mSkullWorld) * R;                           // 计算出镜子中骷髅头的世界坐标
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		mEffect->SetWorld(world);
		mEffect->SetWorldInvTranspose(worldInvTranspose);
		mEffect->SetWorldViewProj(worldViewProj);
		mEffect->SetMaterial(mSkullMat); 

		// Cache the old light directions, and reflect the light directions.         // -- 计算出镜子中的光照
		XMFLOAT3 oldLightDirections[3];
		for (int i = 0; i < 3; ++i)
		{
			oldLightDirections[i] = mDirLights[i].Direction;

			XMVECTOR lightDir = XMLoadFloat3(&mDirLights[i].Direction);
			XMVECTOR reflectedLightDir = XMVector3TransformNormal(lightDir, R);
			XMStoreFloat3(&mDirLights[i].Direction, reflectedLightDir);
		}
		mEffect->SetDirLights(mDirLights);

		// 顺时针的三角形被剔除
		md3dImmediateContext->RSSetState(RenderStates::CullClockwiseRS);
		md3dImmediateContext->OMSetDepthStencilState(RenderStates::DrawReflectionDSS, 1);

		pass->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0); 

		md3dImmediateContext->RSSetState(0);
		md3dImmediateContext->OMSetDepthStencilState(0, 0); 

		// Restore light directions.
		for (int i = 0; i < 3; ++i)
		{
			mDirLights[i].Direction = oldLightDirections[i];
		}
		mEffect->SetDirLights(mDirLights); 
	}
 
	//  绘制镜子,但是要加上半透明混合
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = activeTech->GetPassByIndex(p);
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mRoomVB, &stride, &offset);

		// Set per object constants.
		XMMATRIX world = XMLoadFloat4x4(&mRoomWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		mEffect->SetWorld(world);
		mEffect->SetWorldInvTranspose(worldInvTranspose);
		mEffect->SetWorldViewProj(worldViewProj);
		mEffect->SetTexTransform(XMMatrixIdentity());
		mEffect->SetMaterial(mMirrorMat);
		mEffect->SetDiffuseMap(mMirrorDiffuseMapSRV);

		md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xFFFFFFF);
		pass->Apply(0, md3dImmediateContext);
		md3dImmediateContext->Draw(6, 24);
		md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xFFFFFFFF);
	}
 
	// 绘制骷髅的影子
	activeSkullTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = activeSkullTech->GetPassByIndex(p);

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mSkullVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mSkullIB, DXGI_FORMAT_R32_UINT, 0);

		XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
		XMVECTOR toMainLight = -XMLoadFloat3(&mDirLights[0].Direction);
		XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
		XMMATRIX shadowOffsetY = XMMatrixTranslation(0.0f, 0.001f, 0.0f);

		// Set per object constants.
		XMMATRIX world = XMLoadFloat4x4(&mSkullWorld)*S*shadowOffsetY;
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj; 

		mEffect->SetWorld(world);
		mEffect->SetWorldInvTranspose(worldInvTranspose);
		mEffect->SetWorldViewProj(worldViewProj);
		mEffect->SetMaterial(mShadowMat);

		md3dImmediateContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0);
		pass->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);

		md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
		md3dImmediateContext->OMSetDepthStencilState(0, 0);
	}

	HR(mSwapChain->Present(0, 0)); 
} 

void MirrorApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void MirrorApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void MirrorApp::OnMouseMove(WPARAM btnState, int x, int y)
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
		// Make each pixel correspond to 0.01 unit in the scene.
		float dx = 0.01f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.01f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 3.0f, 50.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void MirrorApp::BuildSkullGeometryBuffers()
{
	std::ifstream fin("../Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"../Models/skull.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertex::PosNormalTexCod> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
	}

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
	vbd.ByteWidth = sizeof(Vertex::PosNormalTexCod) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mSkullVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mSkullIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mSkullIB));
}

void MirrorApp::BuildRoomGeometryBuffers()
{
	// Create and specify geometry.  For this sample we draw a floor
	// and a wall with a mirror on it.  We put the floor, wall, and
	// mirror geometry in one vertex buffer.
	//
	//   |--------------|
	//   |              |
	//   |----|----|----|
	//   |Wall|Mirr|Wall|
	//   |    | or |    |
	//   /--------------/
	//  /   Floor      /
	// /--------------/


	Vertex::PosNormalTexCod v[30];

	// Floor: Observe we tile texture coordinates.
	v[0] = Vertex::PosNormalTexCod(-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	v[1] = Vertex::PosNormalTexCod(-3.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex::PosNormalTexCod(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);

	v[3] = Vertex::PosNormalTexCod(-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	v[4] = Vertex::PosNormalTexCod(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);
	v[5] = Vertex::PosNormalTexCod(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f);

	// Wall: Observe we tile texture coordinates, and that we
	// leave a gap in the middle for the mirror.
	v[6] = Vertex::PosNormalTexCod(-3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[7] = Vertex::PosNormalTexCod(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = Vertex::PosNormalTexCod(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f);

	v[9] = Vertex::PosNormalTexCod(-3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[10] = Vertex::PosNormalTexCod(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f);
	v[11] = Vertex::PosNormalTexCod(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 2.0f);

	v[12] = Vertex::PosNormalTexCod(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[13] = Vertex::PosNormalTexCod(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex::PosNormalTexCod(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);

	v[15] = Vertex::PosNormalTexCod(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[16] = Vertex::PosNormalTexCod(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);
	v[17] = Vertex::PosNormalTexCod(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 2.0f);

	v[18] = Vertex::PosNormalTexCod(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex::PosNormalTexCod(-3.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex::PosNormalTexCod(7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);

	v[21] = Vertex::PosNormalTexCod(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex::PosNormalTexCod(7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);
	v[23] = Vertex::PosNormalTexCod(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 1.0f);

	// Mirror
	v[24] = Vertex::PosNormalTexCod(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[25] = Vertex::PosNormalTexCod(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[26] = Vertex::PosNormalTexCod(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[27] = Vertex::PosNormalTexCod(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[28] = Vertex::PosNormalTexCod(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[29] = Vertex::PosNormalTexCod(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::PosNormalTexCod) * 30;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mRoomVB));
}