// WKEWebkitBrowser.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "WKEWebkitBrowserWnd.h"



void onURLChanged(const struct _wkeClientHandler* clientHandler, const wkeString URL)
{

}

void onTitleChanged(const struct _wkeClientHandler* clientHandler, const wkeString title)
{

}


WKEWebkitBrowserWnd::WKEWebkitBrowserWnd()
{
    pWKEWebkitUI=NULL;
    pURLEditUI=NULL;
}

WKEWebkitBrowserWnd::~WKEWebkitBrowserWnd()
{

}


CControlUI* WKEWebkitBrowserWnd::CreateControl( LPCTSTR pstrClassName )
{
    if (_tcsicmp(pstrClassName, _T("WKEWebkitBrowser")) == 0)
    {
        return  new CWKEWebkitUI;
    }

    return NULL;
}


void WKEWebkitBrowserWnd::InitWindow()
{

    pURLEditUI = dynamic_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("ui_et_urlinput")));

    pWKEWebkitUI = dynamic_cast<CWKEWebkitUI*>(m_PaintManager.FindControl(_T("ui_wke_wkebrowser")));
    if (pWKEWebkitUI)
    {
        static wkeClientHandler hander;
        hander.onTitleChanged = onTitleChanged;
        hander.onURLChanged = onURLChanged;
        pWKEWebkitUI->SetClientHandler(&hander);
        pWKEWebkitUI->LoadFile(_T("Html/index.html"));
    }

}


void WKEWebkitBrowserWnd::Notify( TNotifyUI& msg )
{
    if( msg.sType == _T("click") ) 
    {	
        if( msg.pSender->GetName() == _T("ui_btn_goback") ) 
        {
            if (pWKEWebkitUI)
            {
                pWKEWebkitUI->GoBack();
            }

        }
        if( msg.pSender->GetName() == _T("ui_btn_goforward") ) 
        {
            if (pWKEWebkitUI)
            {
                pWKEWebkitUI->GoForward();
            }

        }
        if( msg.pSender->GetName() == _T("ui_btn_ensure") ) 
        {
            if (pURLEditUI&&pWKEWebkitUI)
            {
                pWKEWebkitUI->LoadURL(pURLEditUI->GetText().GetData());
            }
        }
        if( msg.pSender->GetName() == _T("ui_btn_testjs") ) 
        {
            if (pURLEditUI&&pWKEWebkitUI)
            {
            }
            jsValue jsRet = pWKEWebkitUI->RunJS(pURLEditUI->GetText().GetData());

            //jsToStringW(pURLEditUI->globalExec(), jsRet);

        }
        if( msg.pSender->GetName() == _T("ui_btn_home") ) 
        {
            if (pURLEditUI&&pWKEWebkitUI)
            {
                pWKEWebkitUI->LoadURL(_T(""));

            }
        }
    }
    __super::Notify(msg);
}


LRESULT WKEWebkitBrowserWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

    bHandled=FALSE;

    return 0;
}