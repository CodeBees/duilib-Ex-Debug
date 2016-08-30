// WKEWebkitBrowser.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "WKEWebkitBrowserWnd.h"
#include <strsafe.h>

WKEWebkitBrowserWnd* WKEWebkitBrowserWnd::pWKEWebkitBrowserWnd=NULL;


//typedef jsValue (JS_CALL *jsNativeFunction) (jsExecState es);
jsValue JS_CALL jsInvokeCPlusPlus(jsExecState es)
{
    TCHAR szBuffer[512];
    const wchar_t* strParam1 = jsToStringW(es, jsArg(es, 0));
    const wchar_t* strParam2 = jsToStringW(es, jsArg(es, 1));

   StringCbPrintf(szBuffer,sizeof(szBuffer),_T("jsInvokeCPlusPlus(%s,%s)"),strParam1,strParam2);
   ::MessageBox(GetForegroundWindow(),szBuffer,_T("example"),MB_OK);


   WKEWebkitBrowserWnd* pMainWnd=WKEWebkitBrowserWnd::pWKEWebkitBrowserWnd;

   if (pMainWnd!=NULL)
   {
       if (pMainWnd->pWKEWebkitUI)
       {
           //执行js的例子
           OutputDebugString(pMainWnd->pWKEWebkitUI->RunJS(_T("document.getElementById(\"buttonjs\").hidden=\"true\" ")).c_str());
       }
   }
  
   return jsUndefined();
}


void onURLChangedLocal(const wkeClientHandler* clientHandler, const wkeString wkeStrURL)
{
    WKEWebkitBrowserWnd* pMainWnd=WKEWebkitBrowserWnd::pWKEWebkitBrowserWnd;

    if (pMainWnd!=NULL)
    {

        pMainWnd->strURL_=wkeToStringW(wkeStrURL);
        CLabelUI* pStatusUI=dynamic_cast<CLabelUI*>(pMainWnd->FindControl(_T("ui_lbl_status")));
        if (pStatusUI)
        {
            TCHAR szText[MAX_PATH] = {0};
            _stprintf(szText, _T("Title：%s  URL:%s"), pMainWnd->strTitle_.c_str(),pMainWnd->strURL_.c_str());
            pStatusUI->SetText(szText);
        }

    }

}

void onTitleChangedLocal(const wkeClientHandler* clientHandler, const wkeString title)
{
    WKEWebkitBrowserWnd* pMainWnd=WKEWebkitBrowserWnd::pWKEWebkitBrowserWnd;

    if (pMainWnd!=NULL)
    {
        
        pMainWnd->strTitle_ = wkeToStringW(title);

        CLabelUI* pStatusUI=dynamic_cast<CLabelUI*>(pMainWnd->FindControl(_T("ui_lbl_status")));
        if (pStatusUI)
        {
            TCHAR szText[MAX_PATH] = {0};
            _stprintf(szText, _T("Title：%s  URL:%s"), pMainWnd->strTitle_.c_str(),pMainWnd->strURL_.c_str());
            pStatusUI->SetText(szText);
        }

    }

}


WKEWebkitBrowserWnd::WKEWebkitBrowserWnd()
{
    pWKEWebkitUI=NULL;
    pURLEditUI=NULL;
    pWKEWebkitBrowserWnd=this;
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

        wkeClientHanler_.onTitleChanged = onTitleChangedLocal;
        wkeClientHanler_.onURLChanged = onURLChangedLocal;

        pWKEWebkitUI->SetClientHandler(&wkeClientHanler_);

        jsBindFunction("jsInvokeCPlusPlus", jsInvokeCPlusPlus, 2);//这里绑定js函数，让js主动调用c++函数


        pWKEWebkitUI->LoadFile(_T("htmlexample/index.html"));
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
        if( msg.pSender->GetName() == _T("ui_btn_home") ) 
        {
            if (pURLEditUI&&pWKEWebkitUI)
            {
                pWKEWebkitUI->LoadURL(_T("www.baidu.com"));

            }
        }
    }
    //需要关闭richedit的want return属性
    else if (msg.sType==DUI_MSGTYPE_RETURN)
    {
        if (pURLEditUI==msg.pSender)
        {
            if (pURLEditUI&&pWKEWebkitUI)
            {
                pWKEWebkitUI->LoadURL(pURLEditUI->GetText().GetData());
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