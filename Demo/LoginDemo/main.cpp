#include "stdafx.h"
#include "CLoginWnd.h"
#include "Resource.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	CLoginWindow* pFrame = new CLoginWindow();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->ShowWindow(true);
	pFrame->CenterWindow();
	pFrame->SetIcon(IDI_LOGIN);
	CPaintManagerUI::MessageLoop();

	return 0;
}