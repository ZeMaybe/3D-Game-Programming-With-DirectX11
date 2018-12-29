
#include "D3DApp.h"

int WINAPI WinMain(HINSTANCE h_Inst, HINSTANCE h_PrevInst, LPSTR lpcmdline, int ncmdshow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	D3DApp* theApp = D3DApp::GetApplication();

	if (!theApp)
	{
		MessageBox(0, L"The app is null!", 0, 0);
		return -1;
	}

	if (!theApp->Init(h_Inst))
		return 0;

	return theApp->Run();
}