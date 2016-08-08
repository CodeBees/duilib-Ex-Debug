#pragma once

#include "resource.h"
#include "stdafx.h"
#include "WKEWebkit.h"


class WKEWebkitBrowserWnd : public WindowImplBase
{
public:
    WKEWebkitBrowserWnd();
    ~WKEWebkitBrowserWnd();


    virtual LPCTSTR GetWindowClassName() const      { return _T("WKEWebkitBrowserWnd"); }
    virtual CDuiString GetSkinFile()                { return _T("skin.xml"); }
    virtual CDuiString GetSkinFolder()              { return _T("WKEWebkitBrowserSkin"); }
    virtual CControlUI* CreateControl(LPCTSTR pstrClass);

    virtual void InitWindow();
    virtual void Notify(TNotifyUI& msg);
    virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


private:
    CWKEWebkitUI*	pWKEWebkitUI;
    CRichEditUI*        pURLEditUI;
};

