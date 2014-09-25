// RichList.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "RichList.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例


// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	CPaintManagerUI::SetInstance(hInstance);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	CRichListWnd* pFrame = new CRichListWnd;
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("DuiLib Dem"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 500, 400);
	pFrame->CenterWindow();
	::ShowWindow(pFrame->GetHWND(), SW_SHOW);
	//内存泄露检查
	_CrtDumpMemoryLeaks();
	pFrame->ShowModal();


	::CoUninitialize();
	return 0;

}
//////////////////////////////////////////////////////////////////////////

DUI_BEGIN_MESSAGE_MAP(CRichListWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
	LPCTSTR CRichListWnd::GetWindowClassName( void ) const
{
	return _T("RichListWnd");
}

void CRichListWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

void CRichListWnd::Notify( TNotifyUI &msg )
{
	return WindowImplBase::Notify(msg);
}

void CRichListWnd::InitWindow()
{

}

LRESULT CRichListWnd::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI* pHover = m_PaintManager.FindControl(pt);
	if( pHover == NULL ) return 0;
	/*演示悬停在下载列表的图标上时，动态变换下载图标状态显示*/
	if(pHover->GetName() == _T("down_ico"))
	{
		MessageBox(NULL, _T("鼠标在某控件例如按钮上悬停后，对目标控件操作，这里改变了状态图标大小"), _T("DUILIB DEMO"), MB_OK);
		((CButtonUI *)pHover)->ApplyAttributeList(
			_T("normalimage=\"file='downlist_pause.png' dest='15,9,32,26'\""));                
	}
	return 0;
}


UILIB_RESOURCETYPE CRichListWnd::GetResourceType() const
{
	return UILIB_FILE;
}

void CRichListWnd::OnItemClick( TNotifyUI &msg )
{
	TCHAR alert_msg[64] = {0};
	int index = ((CListContainerElementUI *)msg.pSender)->GetIndex();
	wsprintf(alert_msg, _T("选中了行%d, 查找本行内的下载项目名..."), index);
	MessageBox(NULL, alert_msg, _T("DUILIB DEMO"), MB_OK);            

	CControlUI *find_ctrl =m_PaintManager.FindSubControlByName(msg.pSender, _T("down_name"));

	if(find_ctrl)
	{
		MessageBox(NULL, 
			find_ctrl->GetText()+_T(" 选中行的下载项目名称.."), 
			_T("DUILIB DEMO"), MB_OK);   
		((CLabelUI *)find_ctrl)->SetText(_T("由程序动态设置后的名称..."));
	}
	else
	{
		MessageBox(NULL, _T("本测试行未为控件设置name，故找不到要操作的控件"), 
			_T("DUILIB DEMO"), MB_OK);   
	}

	find_ctrl =m_PaintManager.FindSubControlByName(msg.pSender, _T("down_progress"));

	if(find_ctrl)
	{
		TCHAR alert_msg[256] = {0};
		wsprintf(alert_msg, _T("进度条值:%d"), ((CProgressUI *)find_ctrl)->GetValue());
		MessageBox(NULL, alert_msg, _T("DUILIB DEMO"), MB_OK);   

		((CProgressUI *)find_ctrl)->SetValue(30);
		MessageBox(NULL, _T("修改了进度条值"), _T("DUILIB DEMO"), MB_OK);   
	}
}


void CRichListWnd::OnSelectChanged( TNotifyUI &msg )
{
	if(msg.pSender->GetName() == _T("down_list"))
	{
		static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_main")))->SelectItem(0);
	}
	else if(msg.pSender->GetName() == _T("down_his"))
	{
		static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_main")))->SelectItem(1);
	}
}