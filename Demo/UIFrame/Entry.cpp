#include "stdafx.h"
#include "UIFrame.h"
#include "localfont.h"

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

	//CAddLocalFont* paddLocalFont=new CAddLocalFont(_T("F:\\src-code\\duilib-Ex-Debug\\bin\\UIFrameSkin\\font\\"));
	//paddLocalFont->AddFont(_T("Profontwindows"),nullptr);

	CDuiString str=L"HHHF";
	//CDuiString str=L"abcwoai nimeide   d   HHHabc   ";
	str.Insert(8,'k');
	str.Insert(1,_T("shit"));
	int ncount=str.Remove('H');
	str.Trim();
	str.Trim('a');
	str.Trim(_T("abc"));
	str.TrimLeft();
	str.TrimLeft('a');
	str.TrimLeft(_T("bcwo"));
	str.TrimRight('H');
	

	TCHAR tszModule[MAX_PATH + 1] = { 0 };
	::GetModuleFileName(hInstance, tszModule, MAX_PATH);
	CDuiString sInstancePath = tszModule;
	int pos = sInstancePath.ReverseFind(_T('\\'));
	if( pos >= 0 ) sInstancePath = sInstancePath.Left(pos + 1);

	CDYFrameWnd pFrame;
	CDuiString sFontfilePath=sInstancePath+_T("UIFrameSkin\\Font\\Profontwindows.ttf");
	pFrame.m_paddLocalFont->AddFont(sFontfilePath);
	pFrame.Create(NULL, _T("*UIWindows"), UI_WNDSTYLE_FRAME, 0);
	pFrame.ShowWindow();
	pFrame.CenterWindow();
	pFrame.ShowModal();
	

	//退出程序并释放COM库
	
	::CoUninitialize();

	return 0;
}