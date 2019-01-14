
#ifndef BezierApp_h__
#define BezierApp_h__

#include "D3DApp.h"
#include "D3DEffects.h"
#include "Vertex.h"

class BezierApp : public D3DApp
{
public:
	BezierApp();
	~BezierApp();

	bool Init(HINSTANCE hInstance);
	void DrawScene();

private:
	void BuildQuadPatchBuffer();

private:
	ID3D11Buffer* mQuadPatchVB = 0;

	TessellationEffect* mEffect = 0;
	ID3D11InputLayout* mInputLayout = 0;
	InputLayouts mInputLayouts;
};
#endif // BezierApp_h__