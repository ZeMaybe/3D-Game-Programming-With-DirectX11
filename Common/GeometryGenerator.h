
#ifndef GeometryGenerator_h__
#define GeometryGenerator_h__

#include "d3dUtil.h"

#include <vector>

class GeometryGenerator
{
public:
	struct Vertex
	{
		Vertex() {}
		Vertex(const DirectX::XMFLOAT3& p, 
			const DirectX::XMFLOAT3& n, 
			const DirectX::XMFLOAT3& t, 
			const DirectX::XMFLOAT2& uv);
		Vertex(float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v); 

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 TangentU;
		DirectX::XMFLOAT2 TexC; 
	};

	struct MeshData 
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};

public:
	void CreateBox(float width, float height, float depth, MeshData& meshData);

	void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData);

	void CreateGeosphere(float radius, UINT numSubdivisions, MeshData& meshData);

	void CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);

	void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData);

	void CreateFullscreenQuad(MeshData& meshData);

private:
	void Subdivide(MeshData& meshData);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
}; 

#endif // GeometryGenerator_h__