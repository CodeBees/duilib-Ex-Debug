#include "stdafx.h"
#include "UIFrame.h"


CDYFrameWnd::~CDYFrameWnd()
{
	if (m_paddLocalFont)
	{
		delete m_paddLocalFont;
	}
}

void  CDYFrameWnd::InitWindow()
{
	CButtonUI* pbtn=static_cast<CButtonUI*>(FindControl(_T("ui_btn_maked")));
	if (pbtn)
	{
		pbtn->OnNotify+=MakeDelegate(this,&CDYFrameWnd::OnMsgBtnClick,DUI_MSGTYPE_CLICK);//处理指定的notify类型
		pbtn->OnEvent+= MakeDelegate(this,&CDYFrameWnd::OnMsgBtnMouseEnter,UIEVENT_MOUSEENTER);
		pbtn->OnEvent+= MakeDelegate(this,&CDYFrameWnd::OnMsgBtnMouseLeave,UIEVENT_MOUSELEAVE);//处理指定的事件
		pbtn->OnNotify+=MakeDelegate(this,&CDYFrameWnd::btnOnNotify);//处理所有的notify
	}

	CMediaPlayerUI* pMediaUI = dynamic_cast<CMediaPlayerUI*>(FindControl(_T("MediaplayerActiveX")));
	if (pMediaUI)
	{
		pMediaUI->Play(CPaintManagerUI::GetInstancePath( ) + _T("\\UIFrameSkin\\test.mp4"));
	}


	nsldValue_=10;
}

LRESULT CDYFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	switch (uMsg)
	{

	case WM_TOUCH:
		MessageBoxA(GetHWND(),"get wm_touch message",NULL,MB_OK);

		break;
	default:
		break;
	}
//	__super::MessageHandler(uMsg,wParam,lParam,bHandled);
	return FALSE;
}

class WindowCallback:public IDialogBuilderCallback
{
	virtual CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("Window2")) == 0)
		{
			CDialogBuilder builderManualOpera;
			CControlUI* ptemp = builderManualOpera.Create(_T("Window2.xml"),NULL,NULL,NULL,NULL);
			return ptemp;
		}

		return NULL;
	};

};

CControlUI* CDYFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("Window1")) == 0)
	{
		WindowCallback bk;
		CDialogBuilder builderManualOpera;
		//	传入GetPaintManager(),Window窗口受Window1的size影响
		//CControlUI* pUIManualOpera_ = builderManualOpera.Create(_T("Window1.xml"), NULL, &bk, GetPaintManager(), NULL);
		CControlUI* pUIManualOpera_ = builderManualOpera.Create(_T("Window1.xml"), NULL, &bk, NULL, NULL);
		return pUIManualOpera_;
	}
	if (_tcsicmp(pstrClass, _T("Window2")) == 0)
	{
		CDialogBuilder builderManualOpera;
		CControlUI* ptemp = builderManualOpera.Create(_T("Window2.xml"));
		return ptemp;
	}

	return NULL;
}

void CDYFrameWnd::Notify(TNotifyUI& msg)
{
	char szInfo[128];
	if (msg.sType == DUI_MSGTYPE_COLORCHANGED)
	{
		CColorPaletteUI* p = (CColorPaletteUI*)msg.pSender;
		DWORD color = p->GetSelectColor();
		//DWORD color = msg.wparam;
		sprintf(szInfo, "ARGB=(%d,%d,%d,%d)", 0xFF & color >> 24, 0xFF & color >> 16, 0xFF & color >> 8, 0xFF & color);
		MessageBoxA(GetHWND(), szInfo,"颜色", MB_OK);
		//p->SetSelectColor(0x00f000f0);
		
	}
	else if (msg.sType==DUI_MSGTYPE_SELECTCHANGED)
	{

			if (msg.pSender->GetName()==_T("ut_opt_tab1"))
			{
				CTabLayoutUI* pTab = dynamic_cast<CTabLayoutUI*>(FindControl(_T("ui_tablayer_operaswitch")));
				if (pTab)
				{
					pTab->SelectItem(0);
				}
			}
			else if (msg.pSender->GetName( ) == _T("ut_opt_tab2"))
			{

				CTabLayoutUI* pTab = dynamic_cast<CTabLayoutUI*>(FindControl(_T("ui_tablayer_operaswitch")));
				if (pTab)
				{
					pTab->SelectItem(1);
				}
			}



	}
	else if (msg.sType==DUI_MSGTYPE_WINDOWINIT)
	{

	}

	__super::Notify(msg);
}


bool CDYFrameWnd::OnMsgBtnClick( TNotifyUI* pTNotifyUI )
{
	MessageBox(m_hWnd,_T("绑定的按钮点击消息 OK!"),_T("消息路由"),MB_OK);
	return true;
}



bool CDYFrameWnd::OnMsgBtnMouseEnter( TEventUI* pTEventUI )
{
	
	pTEventUI->pSender->SetText(_T("鼠标进来了"));
	pTEventUI->pSender->SetUserData(_T("鼠标进来了"));

	return true;
}

bool CDYFrameWnd::OnMsgBtnMouseLeave( TEventUI* pTEventUI )
{

	pTEventUI->pSender->SetText(_T("鼠标跑路了"));
	pTEventUI->pSender->SetUserData(_T("鼠标跑路了"));

	CSliderUI* psld=dynamic_cast<CSliderUI*>(FindControl(_T("ui_sld_test")));
	if (psld)
	{
		nsldValue_+=10;
		psld->SetValue(nsldValue_);
		psld->SetText(_T("dddddddd"));
	}
	
	return true;
}


bool CDYFrameWnd::btnOnNotify(void* pNotify)
{

	TNotifyUI* pMsg=static_cast<TNotifyUI*>(pNotify);

	DUITRACE(_T("all notify "));
	return true;
}