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


CRichListWnd::CRichListWnd()
{
	m_Page1.SetPaintMagager(&m_PaintManager);
	m_Page2.SetPaintMagager(&m_PaintManager);
	//关联处理类和界面的virtualwnd属性
	AddVirtualWnd(_T("page1"),&m_Page1);
	AddVirtualWnd(_T("page2"),&m_Page2);
}


CRichListWnd::~CRichListWnd()
{
	RemoveVirtualWnd(_T("page1"));
	RemoveVirtualWnd(_T("page2"));
}

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
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CPage2, CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_END_MESSAGE_MAP()


	CPage2::CPage2()
{
	m_pPaintManager = NULL;
}

void CPage2::SetPaintMagager(CPaintManagerUI* pPaintMgr)
{
	m_pPaintManager = pPaintMgr;
}

void CPage2::OnClick(TNotifyUI& msg)
{
	MessageBox(NULL,_T("消息被路由到此虚拟窗口"),NULL,MB_OK);
}

void CPage2::OnSelectChanged( TNotifyUI &msg )
{

}

void CPage2::OnItemClick( TNotifyUI &msg )
{

}

//////////////////////////////////////////////////////////////////////////
///


DUI_BEGIN_MESSAGE_MAP(CPage1, CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_END_MESSAGE_MAP()

	CPage1::CPage1()
{
	m_pPaintManager = NULL;
}

void CPage1::SetPaintMagager(CPaintManagerUI* pPaintMgr)
{
	m_pPaintManager = pPaintMgr;
}

void CPage1::OnClick(TNotifyUI& msg)
{
	if(msg.pSender->GetName() == _T("down_ico"))
	{                
		CControlUI *find_ctrl =m_pPaintManager->FindSubControlByName(msg.pSender->GetParent()->GetParent(), _T("down_name"));

		if(find_ctrl)
		{
			MessageBox(NULL, 
				find_ctrl->GetText()+_T(" 演示未选中行中的按钮触发动作，依该按钮父结点的找到所属行listcontainer.."), 
				_T("DUILIB DEMO"), MB_OK);   
			((CLabelUI *)find_ctrl)->SetText(_T("由程序动态设置后的名称..."));
		}
	}
	else if(msg.pSender->GetName() == _T("down_del"))
	{
		CListUI *down_list = 
			static_cast<CListUI*>(m_pPaintManager->FindControl(_T("down_list_tab")));
		if(!down_list)
			return;

		down_list->RemoveAt(down_list->GetCurSel());                   
	}
	else if(msg.pSender->GetName() == _T("down_new"))
	{
		CListUI *down_list = static_cast<CListUI*>(m_pPaintManager->FindControl(_T("down_list_tab")));
		if(!down_list)
			return;

		CListContainerElementUI *new_node = new CListContainerElementUI;
		new_node->ApplyAttributeList(_T("height=\"45\""));

		CHorizontalLayoutUI *new_h_lay = new CHorizontalLayoutUI;
		new_h_lay->ApplyAttributeList(_T("float=\"false\" ")\
			_T("childpadding=\"10\" inset=\"3,5,3,5\""));

		CButtonUI *new_btn_1 = new CButtonUI;
		new_btn_1->ApplyAttributeList(
			_T("name=\"down_ico\" float=\"false\" ")\
			_T("bordersize=\"0\" width=\"32\" maxheight=\"26\" ")\
			_T("bkimage=\"downlist_app.png\" ")\
			_T("normalimage=\"file='downlist_run.png' dest='20,14,32,26'\""));

		CVerticalLayoutUI *new_v_lay = new CVerticalLayoutUI;
		new_h_lay->Add(new_btn_1);
		new_h_lay->Add(new_v_lay);

		CLabelUI *new_label = new CLabelUI;
		new_label->ApplyAttributeList(_T("textcolor=\"#FFAAAAAA\" showhtml=\"true\""));
		new_label->SetText(_T("new added item.exe"));
		new_label->SetName(_T("down_name"));
		CProgressUI *new_progress = new CProgressUI;
		new_progress->SetMinValue(0);
		new_progress->SetMaxValue(100);
		new_progress->SetValue(1);
		new_progress->SetMaxWidth(200);
		new_progress->SetMaxHeight(7);
		new_progress->SetForeImage(_T("progress_fore.png"));
		new_progress->SetName(_T("down_progress"));
		new_v_lay->Add(new_label);
		new_v_lay->Add(new_progress);

		CLabelUI *new_label2 = new CLabelUI;
		CLabelUI *new_label3 = new CLabelUI;
		CVerticalLayoutUI *new_v_lay2 = new CVerticalLayoutUI;
		new_h_lay->Add(new_v_lay2);
		new_v_lay2->Add(new_label2);
		new_v_lay2->Add(new_label3);
		new_label2->ApplyAttributeList(
			_T("align=\"right\" text=\"\" textcolor=\"#FFAAAAAA\" showhtml=\"true\""));
		new_label3->ApplyAttributeList(
			_T("align=\"right\" text=\"0.00K/34.33M \" textcolor=\"#FFAAAAAA\" showhtml=\"true\""));

		new_node->Add(new_h_lay);
		down_list->Add(new_node);
	}
}

void CPage1::OnSelectChanged( TNotifyUI &msg )
{

}

void CPage1::OnItemClick( TNotifyUI &msg )
{

}


