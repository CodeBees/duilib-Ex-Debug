#pragma once
#include "duilib.h"

class CFrameWnd: public CXMLWnd
{
public:
    explicit CFrameWnd(LPCTSTR pszXMLPath);

    virtual void InitWindow();
    virtual void Notify(TNotifyUI& msg);
    virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
};