#pragma once
#include "..\..\DuiLib\UIlib.h"
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\..\\lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\..\\lib\\DuiLib.lib")
#   endif
#endif


// 以XML生成界面的窗口基类
class CXMLWnd : public WindowImplBase
{
public:
    explicit CXMLWnd(LPCTSTR pszXMLPath) 
        : m_strXMLPath(pszXMLPath){}

public:
    virtual LPCTSTR GetWindowClassName() const
    {
        return _T("XMLWnd");
    }

    virtual CDuiString GetSkinFile()
    {
        return m_strXMLPath;
    }

    virtual CDuiString GetSkinFolder()
    {
        return _T("Duilib Tutorial");
    }

protected:
    CDuiString m_strXMLPath;
};


// 将HWND显示到CControlUI上面
class CWndUI: public CControlUI
{
public:
    CWndUI(): m_hWnd(NULL){}

    virtual void SetInnerVisible(bool bVisible = true)
    {
        __super::SetInnerVisible(bVisible);
        ::ShowWindow(m_hWnd, bVisible);
    }

    virtual void SetPos(RECT rc)
    {
        __super::SetPos(rc);
        ::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    BOOL Attach(HWND hWndNew)
    {
        if (! ::IsWindow(hWndNew))
        {
            return FALSE;
        }

        m_hWnd = hWndNew;
        return TRUE;
    }

    HWND Detach()
    {
        HWND hWnd = m_hWnd;
        m_hWnd = NULL;
        return hWnd;
    }

protected:
    HWND m_hWnd;
};
