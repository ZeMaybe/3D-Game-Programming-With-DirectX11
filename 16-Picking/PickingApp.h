
#ifndef PickingApp_h__
#define PickingApp_h__

#include "D3DApp.h"

class PickingApp : public D3DApp
{
public:
	PickingApp();
	~PickingApp();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildMeshGeometryBuffers();
	void Pick(int sx, int sy);


};
/*

class PickingApp : public D3DApp
{



private:

ID3D11Buffer* mMeshVB;
ID3D11Buffer* mMeshIB;

// Keep system memory copies of the Mesh geometry for picking.
std::vector<Vertex::Basic32> mMeshVertices;
std::vector<UINT> mMeshIndices;

XNA::AxisAlignedBox mMeshBox;

DirectionalLight mDirLights[3];
Material mMeshMat;
Material mPickedTriangleMat;

// Define transformations from local spaces to world space.
XMFLOAT4X4 mMeshWorld;

UINT mMeshIndexCount;

UINT mPickedTriangle;

Camera mCam;

POINT mLastMousePos;
};
*/
#endif // PickingApp_h__