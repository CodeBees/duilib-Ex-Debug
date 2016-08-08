#include "stdafx.h"
#include "WKEWebkitBrowserWnd.h"



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

    WKEWebkitBrowserWnd pFrame;
    pFrame.Create(NULL, _T("*UIWindows"), UI_WNDSTYLE_FRAME, 0);
    pFrame.CenterWindow();

    //	绘制阴影

    CShadowWindow shadowwnd;
    CShadowWindow::Initialize(hInstance);
    shadowwnd.Create(pFrame.GetHWND());
    shadowwnd.SetSize(4);
    shadowwnd.SetPosition(4,4);
    shadowwnd.SetColor(RGB(22,22,22));

    pFrame.ShowModal();


    //退出程序并释放COM库
    ::CoUninitialize();

    return 0;
}