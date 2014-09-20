#include "stdafx.h"
#include "UIFrame.h"


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//实例句柄与渲染类关联
	CPaintManagerUI::SetInstance(hInstance);

	//Initializes the COM library on the current thread and identifies,初始化COM库, 为加载COM库提供支持
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr))
	{
		return 0;
	}

	//CDYFrameWnd *pFrame = new CDYFrameWnd();
	//pFrame->Create(NULL, _T("*UIWindows"), UI_WNDSTYLE_FRAME, 0);
	//pFrame->ShowWindow();
	//pFrame->CenterWindow();
	//pFrame->ShowModal();

	CDYFrameWnd pFrame;
	pFrame.Create(NULL, _T("*UIWindows"), UI_WNDSTYLE_FRAME, 0);
	pFrame.ShowWindow();
	pFrame.CenterWindow();
	pFrame.ShowModal();
	//退出程序并释放COM库
	
	::CoUninitialize();

	return 0;
}