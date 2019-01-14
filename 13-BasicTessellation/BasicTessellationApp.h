
#ifndef BasicTessellationApp_h__
#define BasicTessellationApp_h__

#include "D3DApp.h"
#include "Vertex.h"
#include "D3DEffects.h"

class BasicTessellationApp : public D3DApp
{
public:
	BasicTessellationApp();
	~BasicTessellationApp();

	bool Init(HINSTANCE hinst);
	void DrawScene();

private:
	void BuildQuadPatchBuffer();

private:
	ID3D11Buffer* mQuadPatchVB = 0;

	TessellationEffect* mEffect = 0;
	ID3D11InputLayout* mInputLayout = 0;
	InputLayouts mInputLayouts;
}; 

#endif // BasicTessellationApp_h__